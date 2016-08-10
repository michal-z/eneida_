#include "eneida.h"
#include "eneida_lib.cpp"

// needed by VC when CRT is not used (/NODEFAULTLIBS)
extern "C" { int32_t _fltused; }

int64_t STDCALL Demo::WindowsMessageHandler(void *Window, uint32_t Message, uint64_t Param1, int64_t Param2)
{
    switch (Message)
    {
        case WM_DESTROY:
        case WM_KEYDOWN:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(Window, Message, Param1, Param2);
}

int32_t Demo::Initialize()
{
    m_Kernel32 = LoadLibraryA("kernel32.dll");
    m_User32 = LoadLibraryA("user32.dll");
    m_Gdint32_t = LoadLibraryA("gdint32_t.dll");
    m_Dxgi = LoadLibraryA("dxgi.dll");
    m_D3D12 = LoadLibraryA("d3d12.dll");

    OutputDebugString = (OutputDebugString_fn)GetProcAddress(m_Kernel32, "OutputDebugStringA");
    ExitProcess = (ExitProcess_fn)GetProcAddress(m_Kernel32, "ExitProcess");
    GetModuleHandle = (GetModuleHandle_fn)GetProcAddress(m_Kernel32, "GetModuleHandleA");
    Sleep = (Sleep_fn)GetProcAddress(m_Kernel32, "Sleep");
    CreateEventEx = (CreateEventEx_fn)GetProcAddress(m_Kernel32, "CreateEventExA");
    WaitForSingleObject = (WaitForSingleObject_fn)GetProcAddress(m_Kernel32, "WaitForSingleObject");
    QueryPerformanceCounter = (QueryPerformanceCounter_fn)GetProcAddress(m_Kernel32, "QueryPerformanceCounter");
    QueryPerformanceFrequency = (QueryPerformanceFrequency_fn)GetProcAddress(m_Kernel32, "QueryPerformanceFrequency");

    PeekMessage = (PeekMessage_fn)GetProcAddress(m_User32, "PeekMessageA");
    DispatchMessage = (DispatchMessage_fn)GetProcAddress(m_User32, "DispatchMessageA");
    PostQuitMessage = (PostQuitMessage_fn)GetProcAddress(m_User32, "PostQuitMessage");
    DefWindowProc = (DefWindowProc_fn)GetProcAddress(m_User32, "DefWindowProcA");
    LoadCursor = (LoadCursor_fn)GetProcAddress(m_User32, "LoadCursorA");
    RegisterClass = (RegisterClass_fn)GetProcAddress(m_User32, "RegisterClassA");
    CreateWindowEx = (CreateWindowEx_fn)GetProcAddress(m_User32, "CreateWindowExA");
    AdjustWindowRect = (AdjustWindowRect_fn)GetProcAddress(m_User32, "AdjustWindowRect");
    wsprintf = (wsprintf_fn)GetProcAddress(m_User32, "wsprintfA");
    SetWindowText = (SetWindowText_fn)GetProcAddress(m_User32, "SetWindowTextA");

    CreateDXGIFactory1 = (CreateDXGIFactory1_fn)GetProcAddress(m_Dxgi, "CreateDXGIFactory1");

    D3D12GetDebugInterface = (D3D12GetDebugInterface_fn)GetProcAddress(m_D3D12, "D3D12GetDebugInterface");
    D3D12CreateDevice = (D3D12CreateDevice_fn)GetProcAddress(m_D3D12, "D3D12CreateDevice");

#ifdef _DEBUG
    ID3D12Debug *dbg = nullptr;
    D3D12GetDebugInterface(IID_ID3D12Debug, (void **)&dbg);
    if (dbg)
    {
        dbg->EnableDebugLayer();
        COMRELEASE(dbg);
    }
#endif

    m_Resolution[0] = kDemoResX;
    m_Resolution[1] = kDemoResY;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowsMessageHandler;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = kDemoName;
    if (!RegisterClass(&wc)) return 0;

    RECT rect = { 0, 0, (int32_t)m_Resolution[0], (int32_t)m_Resolution[1] };
    if (!AdjustWindowRect(&rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, FALSE)) return 0;

    m_Window = CreateWindowEx(0, kDemoName, kDemoName,
                              WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              rect.right - rect.left, rect.bottom - rect.top,
                              nullptr, nullptr, nullptr, 0);
    Assert(m_Window);

 
    IDXGIFactory4* factory_dxgi = nullptr;
    COMCHECK(CreateDXGIFactory1(IID_IDXGIFactory4, (void**)&factory_dxgi));
    COMCHECK(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_ID3D12Device, (void**)&m_Gpu));

    D3D12_COMMAND_QUEUE_DESC cmd_queue_desc = {};
    cmd_queue_desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmd_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmd_queue_desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    COMCHECK(m_Gpu->CreateCommandQueue(&cmd_queue_desc, IID_ID3D12CommandQueue, (void**)&m_CmdQueue));

    DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
    swapchain_desc.BufferCount       = kNumSwapbuffers;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow      = m_Window;
    swapchain_desc.SampleDesc.Count  = 1;
    swapchain_desc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapchain_desc.Windowed          = (kDemoFullscreen ? FALSE : TRUE);

    IDXGISwapChain *swapchain = nullptr;
    COMCHECK(factory_dxgi->CreateSwapChain(m_CmdQueue, &swapchain_desc, &swapchain));
    COMCHECK(swapchain->QueryInterface(IID_IDXGISwapChain3, (void**)&m_Swapchain));
    COMRELEASE(swapchain);
    COMRELEASE(factory_dxgi);

    m_RtvSize = m_Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    m_CbvSrvUavSize = m_Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.NumDescriptors = kNumSwapbuffers;
    rtv_heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    COMCHECK(m_Gpu->CreateDescriptorHeap(&rtv_heap_desc, IID_ID3D12DescriptorHeap, (void**)&m_RtvHeap));
    m_RtvHeapStart = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = m_RtvHeapStart;

    for (uint32_t i = 0; i < kNumSwapbuffers; ++i)
    {
        COMCHECK(m_Swapchain->GetBuffer(i, IID_ID3D12Resource, (void**)&m_Swapbuffers[i]));

        m_Gpu->CreateRenderTargetView(m_Swapbuffers[i], nullptr, rtv_handle);
        rtv_handle.ptr += m_RtvSize;
    }

    m_Viewport = { 0.0f, 0.0f, (float)m_Resolution[0], (float)m_Resolution[1], 0.0f, 1.0f };
    m_ScissorRect = { 0, 0, (int32_t)m_Resolution[0], (int32_t)m_Resolution[1] };


    for (uint32_t i = 0; i < kNumBufferedFrames; ++i)
    {
        m_FrameResources[i].Create(m_Gpu);
    }

    m_FrameFenceValue = 0;
    COMCHECK(m_Gpu->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_ID3D12Fence, (void**)&m_FrameFence));
    m_FrameFenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    Assert(m_FrameFenceEvent);


    COMCHECK(m_Gpu->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_FrameResources[0].m_CmdAlloc,
                                      nullptr, IID_ID3D12GraphicsCommandList, (void**)&m_CmdList));
    return 1;
}

void Demo::Shutdown()
{
    WaitForGpu();
    //ShutdownDemo(Demo);

    COMRELEASE(m_Swapchain);
    COMRELEASE(m_CmdQueue);
    COMRELEASE(m_Gpu);
}

void Demo::WaitForGpu()
{
    if (m_CmdQueue && m_FrameFence)
    {
        m_CmdQueue->Signal(m_FrameFence, ++m_FrameFenceValue);

        if (m_FrameFence->GetCompletedValue() < m_FrameFenceValue)
        {
            m_FrameFence->SetEventOnCompletion(m_FrameFenceValue, m_FrameFenceEvent);
            WaitForSingleObject(m_FrameFenceEvent, INFINITE);
        }
    }
}
/*
static void
UpdateScene1(demo_state *Demo)
{
    frame_resources *FrameRes = &Demo->FrameResources[Demo->FrameIndex];
    ID3D12GraphicsCommandList *CmdList = Demo->CmdList;

    FrameRes->CmdAlloc->Reset();

    CmdList->Reset(FrameRes->CmdAlloc, nullptr);
    //cmdlist->SetDescriptorHeaps(1, &fres->dheap);

    CmdList->RSSetViewports(1, &Demo->Viewport);
    CmdList->RSSetScissorRects(1, &Demo->ScissorRect);

    TransitionBarrier(CmdList, Demo->Swapbuffers[Demo->SwapbufferIndex], D3D12_RESOURCE_STATE_PRESENT,
                      D3D12_RESOURCE_STATE_RENDER_TARGET);

    float ClearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };

    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle = Demo->RtvHeapStart;
    RtvHandle.ptr += Demo->SwapbufferIndex * Demo->RtvSize;

    //cmdlist->SetGraphicsRootSignature(demo->root_signature);
    //cmdlist->SetGraphicsRootDescriptorTable(0, fres->gpu_dheap_start);

    CmdList->ClearRenderTargetView(RtvHandle, ClearColor, 0, nullptr);
    CmdList->OMSetRenderTargets(1, &RtvHandle, TRUE, nullptr);
    //cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //ID3D12GraphicsCommandList_DrawInstanced(demo->cmd_list, 3, 1, 0, 0);

    TransitionBarrier(Demo->CmdList, Demo->Swapbuffers[Demo->SwapbufferIndex],
                      D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    CmdList->Close();
}
*/

void Demo::Run()
{

    UpdateFrameStats(m_Window, &m_Time, &m_TimeDelta);
    //UpdateDemo(Demo);

    m_Swapchain->Present(0, 0);

    const uint64_t CpuValue = ++m_FrameFenceValue;
    m_CmdQueue->Signal(m_FrameFence, CpuValue);

    const uint64_t GpuValue = m_FrameFence->GetCompletedValue();

    if ((CpuValue - GpuValue) >= kNumBufferedFrames)
    {
        m_FrameFence->SetEventOnCompletion(GpuValue + 1, m_FrameFenceEvent);
        WaitForSingleObject(m_FrameFenceEvent, INFINITE);
    }

    m_SwapbufferIndex = m_Swapchain->GetCurrentBackBufferIndex();
    m_FrameIndex = ++m_FrameIndex % kNumBufferedFrames;
}

void FrameResources::Create(ID3D12Device* gpu)
{
    // command allocator
    COMCHECK(gpu->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                         IID_ID3D12CommandAllocator, (void**)&m_CmdAlloc));

    // GPU visible decriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
    heap_desc.NumDescriptors = kNumGpuDescriptors;
    heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heap_desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    COMCHECK(gpu->CreateDescriptorHeap(&heap_desc, IID_ID3D12DescriptorHeap, (void**)&m_Heap));

    m_HeapCpuStart = m_Heap->GetCPUDescriptorHandleForHeapStart();
    m_HeapGpuStart = m_Heap->GetGPUDescriptorHandleForHeapStart();


    // constant buffer
    D3D12_HEAP_PROPERTIES HeapProps = {};
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC BufferDesc = {};
    BufferDesc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
    BufferDesc.Width            = 64 * 1024;
    BufferDesc.Height           = 1;
    BufferDesc.DepthOrArraySize = 1;
    BufferDesc.MipLevels        = 1;
    BufferDesc.SampleDesc.Count = 1;
    BufferDesc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    COMCHECK(gpu->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                          D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                          IID_ID3D12Resource, (void **)&m_Cb));
}

void Start()
{
    Demo demo = {};

    if (!demo.Initialize())
    {
        ExitProcess(1);
    }

    MSG Message = {};
    for (;;)
    {
        if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Message);
            if (Message.message == WM_QUIT) break;
        }
        else
        {
        }
    }

    ExitProcess(0);
}
