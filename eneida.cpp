#include "eneida_windows.hpp"

// shaders
#include "s00.h"
#include "s01.h"
#include "s02.h"

// TODO: Finish Assert implementation
#ifdef _DEBUG
#define Assert(Expression) if (!(Expression)) { __debugbreak(); }
#else
#define Assert(Expression)
#endif

#define COMRELEASE(comobj) if ((comobj)) { (comobj)->Release(); (comobj) = nullptr; }
#define COMCHECK(r) if ((r) != 0) { __debugbreak(); }

#define k_DemoName "eneida"
#define k_DemoRes 1024
#define k_DemoFullscreen 0

#define k_NumSwapbuffers 4
#define k_NumBufferedFrames 3
#define k_NumGpuDescriptors 1000

struct FrameResources
{
    ID3D12CommandAllocator*     m_CmdAlloc;
    ID3D12Resource*             m_Cb;
    void*                       m_CbCpuAddr;
    D3D12_GPU_VIRTUAL_ADDRESS   m_CbGpuAddr;
    ID3D12DescriptorHeap*       m_Heap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_HeapCpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE m_HeapGpuStart;
};

// global state
static struct
{
    uint32_t                    m_FrameIndex;
    double                      m_Time;
    float                       m_TimeDelta;
    ID3D12Device*               m_Gpu;
    ID3D12CommandQueue*         m_CmdQueue;
    ID3D12GraphicsCommandList*  m_CmdList;
    uint32_t                    m_RtvSize;
    uint32_t                    m_SwapbufferIndex;
    ID3D12Resource*             m_Swapbuffers[k_NumSwapbuffers];
    uint32_t                    m_CbvSrvUavSize;
    D3D12_VIEWPORT              m_Viewport;
    D3D12_RECT                  m_ScissorRect;
    ID3D12DescriptorHeap*       m_RtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RtvHeapStart;
    FrameResources              m_FrameResources[k_NumBufferedFrames];
    IDXGISwapChain3*            m_Swapchain;
    void*                       m_Window;
    ID3D12Fence*                m_FrameFence;
    uint64_t                    m_FrameFenceValue;
    void*                       m_FrameFenceEvent;
    void*                       m_Kernel32;
    void*                       m_User32;
    void*                       m_Dxgi;
    void*                       m_D3D12;
    ID3D12Resource*             m_TargetTex;
    ID3D12PipelineState*        m_DisplayPso;
    ID3D12PipelineState*        m_ComputePso;
    ID3D12RootSignature*        m_DisplayRs;
    ID3D12RootSignature*        m_ComputeRs;
} S;

static void FlushGpu();

// defined in eneida_asmlib.asm
extern "C" void* memset(void* dest, int32_t value, size_t length);

// needed by VC when CRT is not used (/NODEFAULTLIBS)
extern "C" { int32_t _fltused; }

static double
GetTime()
{
    static int64_t freq;
    static int64_t start_counter;

    if (freq == 0)
    {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start_counter);
    }
    int64_t counter;
    QueryPerformanceCounter(&counter);
    return (counter - start_counter) / (double)freq;
}

static void
UpdateFrameStats(void* win, double* time, float* time_delta)
{
    static double prev_time = -1.0;
    static double prev_fps_time = 0.0;
    static uint32_t fps_frame = 0;

    if (prev_time < 0.0)
    {
        prev_time = GetTime();
        prev_fps_time = prev_time;
    }

    *time = GetTime();
    *time_delta = (float)(*time - prev_time);
    prev_time = *time;

    if ((*time - prev_fps_time) >= 1.0)
    {
        double fps = fps_frame / (*time - prev_fps_time);
        double micro_sec = (1.0 / fps) * 1000000.0;
        char text[256];
        wsprintf(text, "[%d fps  %d us] %s", (int32_t)fps, (int32_t)micro_sec, k_DemoName);
        SetWindowText(win, text);
        prev_fps_time = *time;
        fps_frame = 0;
    }
    fps_frame++;
}

static void
TransitionBarrier(ID3D12GraphicsCommandList* cmdlist, ID3D12Resource* resource,
                  D3D12_RESOURCE_STATES state_before, D3D12_RESOURCE_STATES state_after)
{
    D3D12_RESOURCE_BARRIER desc = {};
    desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    desc.Transition.pResource = resource;
    desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    desc.Transition.StateBefore = state_before;
    desc.Transition.StateAfter = state_after;
    cmdlist->ResourceBarrier(1, &desc);
}

static int64_t STDCALL
WindowsMessageHandler(void *window, uint32_t message, uint64_t param1, int64_t param2)
{
    switch (message)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
            if (param1 == VK_ESCAPE)
            {
                PostQuitMessage(0);
                return 0;
            }
            else if (param1 == 'Q')
            {
                PostQuitMessage(1);
                return 0;
            }
            break;
    }
    return DefWindowProc(window, message, param1, param2);
}

static void
CreateFrameResources(uint32_t frame_idx)
{
    Assert(frame_idx < k_NumBufferedFrames);

    FrameResources* fr = &S.m_FrameResources[frame_idx];

    // command allocator
    COMCHECK(S.m_Gpu->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                             IID_ID3D12CommandAllocator, (void**)&fr->m_CmdAlloc));

    // GPU visible decriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
    heap_desc.NumDescriptors = k_NumGpuDescriptors;
    heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heap_desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    COMCHECK(S.m_Gpu->CreateDescriptorHeap(&heap_desc, IID_ID3D12DescriptorHeap,
                                           (void**)&fr->m_Heap));

    fr->m_HeapCpuStart = fr->m_Heap->GetCPUDescriptorHandleForHeapStart();
    fr->m_HeapGpuStart = fr->m_Heap->GetGPUDescriptorHandleForHeapStart();


    // constant buffer
    D3D12_HEAP_PROPERTIES heap_props = {};
    heap_props.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC buffer_desc = {};
    buffer_desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
    buffer_desc.Width            = 64 * 1024;
    buffer_desc.Height           = 1;
    buffer_desc.DepthOrArraySize = 1;
    buffer_desc.MipLevels        = 1;
    buffer_desc.SampleDesc.Count = 1;
    buffer_desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    COMCHECK(S.m_Gpu->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &buffer_desc,
                                              D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                              IID_ID3D12Resource, (void**)&fr->m_Cb));
}

static void
CreatePipelines()
{
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
        desc.VS = { s_s00, sizeof(s_s00) };
        desc.PS = { s_s01, sizeof(s_s01) };
        desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        desc.SampleMask = 0xffffffff;
        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;

        COMCHECK(S.m_Gpu->CreateGraphicsPipelineState(&desc, IID_ID3D12PipelineState, (void**)&S.m_DisplayPso));
        COMCHECK(S.m_Gpu->CreateRootSignature(0, s_s01, sizeof(s_s01), IID_ID3D12RootSignature, (void**)&S.m_DisplayRs));
    }
    {
        D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
        desc.CS = { s_s02, sizeof(s_s02) };

        COMCHECK(S.m_Gpu->CreateComputePipelineState(&desc, IID_ID3D12PipelineState, (void**)&S.m_ComputePso));
        COMCHECK(S.m_Gpu->CreateRootSignature(0, s_s02, sizeof(s_s02), IID_ID3D12RootSignature, (void**)&S.m_ComputeRs));
    }
}

static int32_t
Initialize()
{
#ifdef _DEBUG
    ID3D12Debug* dbg = nullptr;
    D3D12GetDebugInterface(IID_ID3D12Debug, (void**)&dbg);
    if (dbg)
    {
        //dbg->EnableDebugLayer();
        COMRELEASE(dbg);
    }
#endif

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowsMessageHandler;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = k_DemoName;
    if (!RegisterClass(&wc)) {
        Assert(0);
    }

    RECT rect = { 0, 0, (int32_t)k_DemoRes, (int32_t)k_DemoRes };
    if (!AdjustWindowRect(&rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, FALSE)) return 0;

    S.m_Window = CreateWindowEx(0, k_DemoName, k_DemoName,
                                WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                rect.right - rect.left, rect.bottom - rect.top,
                                nullptr, nullptr, nullptr, 0);
    Assert(S.m_Window);

 
    IDXGIFactory4* factory_dxgi = nullptr;
    COMCHECK(CreateDXGIFactory1(IID_IDXGIFactory4, (void**)&factory_dxgi));
    COMCHECK(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_ID3D12Device, (void**)&S.m_Gpu));

    D3D12_COMMAND_QUEUE_DESC cmd_queue_desc = {};
    cmd_queue_desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmd_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmd_queue_desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    COMCHECK(S.m_Gpu->CreateCommandQueue(&cmd_queue_desc, IID_ID3D12CommandQueue, (void**)&S.m_CmdQueue));

    DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
    swapchain_desc.BufferCount       = k_NumSwapbuffers;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow      = S.m_Window;
    swapchain_desc.SampleDesc.Count  = 1;
    swapchain_desc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapchain_desc.Windowed          = (k_DemoFullscreen ? FALSE : TRUE);

    IDXGISwapChain *swapchain = nullptr;
    COMCHECK(factory_dxgi->CreateSwapChain(S.m_CmdQueue, &swapchain_desc, &swapchain));
    COMCHECK(swapchain->QueryInterface(IID_IDXGISwapChain3, (void**)&S.m_Swapchain));
    COMRELEASE(swapchain);
    COMRELEASE(factory_dxgi);

    S.m_RtvSize = S.m_Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    S.m_CbvSrvUavSize = S.m_Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.NumDescriptors = k_NumSwapbuffers;
    rtv_heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    COMCHECK(S.m_Gpu->CreateDescriptorHeap(&rtv_heap_desc, IID_ID3D12DescriptorHeap, (void**)&S.m_RtvHeap));
    S.m_RtvHeapStart = S.m_RtvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = S.m_RtvHeapStart;

    for (uint32_t i = 0; i < k_NumSwapbuffers; ++i)
    {
        COMCHECK(S.m_Swapchain->GetBuffer(i, IID_ID3D12Resource, (void**)&S.m_Swapbuffers[i]));

        S.m_Gpu->CreateRenderTargetView(S.m_Swapbuffers[i], nullptr, rtv_handle);
        rtv_handle.ptr += S.m_RtvSize;
    }

    S.m_Viewport = { 0.0f, 0.0f, (float)k_DemoRes, (float)k_DemoRes, 0.0f, 1.0f };
    S.m_ScissorRect = { 0, 0, (int32_t)k_DemoRes, (int32_t)k_DemoRes };


    D3D12_HEAP_PROPERTIES heap_props = {};
    heap_props.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC target_desc = {};
    target_desc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    target_desc.Width            = k_DemoRes;
    target_desc.Height           = k_DemoRes;
    target_desc.DepthOrArraySize = 1;
    target_desc.MipLevels        = 1;
    target_desc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
    target_desc.SampleDesc.Count = 1;
    target_desc.Layout           = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    target_desc.Flags            = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    COMCHECK(S.m_Gpu->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &target_desc,
                                              D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
                                              IID_ID3D12Resource, (void**)&S.m_TargetTex));

    CreatePipelines();

    for (uint32_t i = 0; i < k_NumBufferedFrames; ++i)
    {
        CreateFrameResources(i);

        D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = S.m_FrameResources[i].m_HeapCpuStart;

        D3D12_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
        uav_desc.Format               = DXGI_FORMAT_R32G32B32A32_FLOAT;
        uav_desc.ViewDimension        = D3D12_UAV_DIMENSION_TEXTURE2D;
        uav_desc.Texture2D.MipSlice   = 0;
        uav_desc.Texture2D.PlaneSlice = 0;
        S.m_Gpu->CreateUnorderedAccessView(S.m_TargetTex, nullptr, &uav_desc, cpu_handle);

        cpu_handle.ptr += S.m_CbvSrvUavSize;

        D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
        srv_desc.Format                    = DXGI_FORMAT_R32G32B32A32_FLOAT;
        srv_desc.ViewDimension             = D3D12_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        srv_desc.Texture2D.MostDetailedMip = 0;
        srv_desc.Texture2D.MipLevels       = 1;
        S.m_Gpu->CreateShaderResourceView(S.m_TargetTex, &srv_desc, cpu_handle);
    }


    S.m_FrameFenceValue = 0;
    COMCHECK(S.m_Gpu->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_ID3D12Fence, (void**)&S.m_FrameFence));
    S.m_FrameFenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    Assert(S.m_FrameFenceEvent);


    COMCHECK(S.m_Gpu->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, S.m_FrameResources[0].m_CmdAlloc,
                                        nullptr, IID_ID3D12GraphicsCommandList, (void**)&S.m_CmdList));
    S.m_CmdList->Close();

    return 1;
}

static void
Shutdown()
{
    COMRELEASE(S.m_Swapchain);
    COMRELEASE(S.m_CmdQueue);
    COMRELEASE(S.m_Gpu);
}

static void
Update()
{
    FrameResources* fres = &S.m_FrameResources[S.m_FrameIndex];

    fres->m_CmdAlloc->Reset();

    S.m_CmdList->Reset(fres->m_CmdAlloc, S.m_ComputePso);
    S.m_CmdList->RSSetViewports(1, &S.m_Viewport);
    S.m_CmdList->RSSetScissorRects(1, &S.m_ScissorRect);

    S.m_CmdList->SetDescriptorHeaps(1, &fres->m_Heap);


    D3D12_GPU_DESCRIPTOR_HANDLE target_handle = fres->m_HeapGpuStart;

    S.m_CmdList->SetComputeRootSignature(S.m_ComputeRs);
    S.m_CmdList->SetComputeRootDescriptorTable(0, target_handle);
    S.m_CmdList->Dispatch(k_DemoRes / 16, k_DemoRes / 16, 1);


    TransitionBarrier(S.m_CmdList, S.m_TargetTex, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    TransitionBarrier(S.m_CmdList, S.m_Swapbuffers[S.m_SwapbufferIndex], D3D12_RESOURCE_STATE_PRESENT,
                      D3D12_RESOURCE_STATE_RENDER_TARGET);


    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = S.m_RtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtv_handle.ptr += S.m_SwapbufferIndex * S.m_RtvSize;
    S.m_CmdList->OMSetRenderTargets(1, &rtv_handle, FALSE, nullptr);

    target_handle.ptr += S.m_CbvSrvUavSize;

    S.m_CmdList->SetPipelineState(S.m_DisplayPso);
    S.m_CmdList->SetGraphicsRootSignature(S.m_DisplayRs);
    S.m_CmdList->SetGraphicsRootDescriptorTable(0, target_handle);

    S.m_CmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    S.m_CmdList->DrawInstanced(3, 1, 0, 0);


    TransitionBarrier(S.m_CmdList, S.m_TargetTex, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
                      D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

    TransitionBarrier(S.m_CmdList, S.m_Swapbuffers[S.m_SwapbufferIndex], D3D12_RESOURCE_STATE_RENDER_TARGET,
                      D3D12_RESOURCE_STATE_PRESENT);

    S.m_CmdList->Close();
    S.m_CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&S.m_CmdList);
}

static void
FlushGpu()
{
    if (S.m_CmdQueue && S.m_FrameFence)
    {
        S.m_CmdQueue->Signal(S.m_FrameFence, ++S.m_FrameFenceValue);

        if (S.m_FrameFence->GetCompletedValue() < S.m_FrameFenceValue)
        {
            S.m_FrameFence->SetEventOnCompletion(S.m_FrameFenceValue, S.m_FrameFenceEvent);
            WaitForSingleObject(S.m_FrameFenceEvent, INFINITE);
        }
    }
}

static int32_t
Run()
{
    if (!Initialize())
    {
        Shutdown();
        return 2;
    }


    for (;;)
    {
        MSG msg = {};
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                Shutdown();
                return (int32_t)msg.wParam;
            }
        }

        UpdateFrameStats(S.m_Window, &S.m_Time, &S.m_TimeDelta);
        Update();

        S.m_Swapchain->Present(0, 0);

        const uint64_t CpuValue = ++S.m_FrameFenceValue;
        S.m_CmdQueue->Signal(S.m_FrameFence, CpuValue);

        const uint64_t GpuValue = S.m_FrameFence->GetCompletedValue();

        if ((CpuValue - GpuValue) >= k_NumBufferedFrames)
        {
            S.m_FrameFence->SetEventOnCompletion(GpuValue + 1, S.m_FrameFenceEvent);
            WaitForSingleObject(S.m_FrameFenceEvent, INFINITE);
        }

        S.m_SwapbufferIndex = S.m_Swapchain->GetCurrentBackBufferIndex();
        S.m_FrameIndex = ++S.m_FrameIndex % k_NumBufferedFrames;
    }
}

void
Start()
{
    S.m_Kernel32 = LoadLibraryA("kernel32.dll");
    S.m_User32 = LoadLibraryA("user32.dll");
    S.m_Dxgi = LoadLibraryA("dxgi.dll");
    S.m_D3D12 = LoadLibraryA("d3d12.dll");

    OutputDebugString = (OutputDebugString_fn)GetProcAddress(S.m_Kernel32, "OutputDebugStringA");
    ExitProcess = (ExitProcess_fn)GetProcAddress(S.m_Kernel32, "ExitProcess");
    GetModuleHandle = (GetModuleHandle_fn)GetProcAddress(S.m_Kernel32, "GetModuleHandleA");
    Sleep = (Sleep_fn)GetProcAddress(S.m_Kernel32, "Sleep");
    CreateEventEx = (CreateEventEx_fn)GetProcAddress(S.m_Kernel32, "CreateEventExA");
    WaitForSingleObject = (WaitForSingleObject_fn)GetProcAddress(S.m_Kernel32, "WaitForSingleObject");
    QueryPerformanceCounter = (QueryPerformanceCounter_fn)GetProcAddress(S.m_Kernel32, "QueryPerformanceCounter");
    QueryPerformanceFrequency = (QueryPerformanceFrequency_fn)GetProcAddress(S.m_Kernel32, "QueryPerformanceFrequency");
    VirtualAlloc = (VirtualAlloc_fn)GetProcAddress(S.m_Kernel32, "VirtualAlloc");
    VirtualFree = (VirtualFree_fn)GetProcAddress(S.m_Kernel32, "VirtualFree");

    PeekMessage = (PeekMessage_fn)GetProcAddress(S.m_User32, "PeekMessageA");
    DispatchMessage = (DispatchMessage_fn)GetProcAddress(S.m_User32, "DispatchMessageA");
    PostQuitMessage = (PostQuitMessage_fn)GetProcAddress(S.m_User32, "PostQuitMessage");
    DefWindowProc = (DefWindowProc_fn)GetProcAddress(S.m_User32, "DefWindowProcA");
    LoadCursor = (LoadCursor_fn)GetProcAddress(S.m_User32, "LoadCursorA");
    RegisterClass = (RegisterClass_fn)GetProcAddress(S.m_User32, "RegisterClassA");
    CreateWindowEx = (CreateWindowEx_fn)GetProcAddress(S.m_User32, "CreateWindowExA");
    AdjustWindowRect = (AdjustWindowRect_fn)GetProcAddress(S.m_User32, "AdjustWindowRect");
    wsprintf = (wsprintf_fn)GetProcAddress(S.m_User32, "wsprintfA");
    SetWindowText = (SetWindowText_fn)GetProcAddress(S.m_User32, "SetWindowTextA");

    CreateDXGIFactory1 = (CreateDXGIFactory1_fn)GetProcAddress(S.m_Dxgi, "CreateDXGIFactory1");

    D3D12GetDebugInterface = (D3D12GetDebugInterface_fn)GetProcAddress(S.m_D3D12, "D3D12GetDebugInterface");
    D3D12CreateDevice = (D3D12CreateDevice_fn)GetProcAddress(S.m_D3D12, "D3D12CreateDevice");


    const int32_t exit_code = Run();
    ExitProcess(exit_code);
}
