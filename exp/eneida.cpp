#include "eneida.h"
#include "eneida_lib.cpp"

// needed by VC when CRT is not used (/NODEFAULTLIBS)
extern "C" { i32 _fltused; }



static i64 STDCALL WindowsMessageHandler(void *Window, u32 Message, u64 Param1, i64 Param2)
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

i32 Demo::Initialize()
{
    WNDCLASS Winclass = {};
    Winclass.lpfnWndProc = WindowsMessageHandler;
    Winclass.hInstance = GetModuleHandle(nullptr);
    Winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    Winclass.lpszClassName = kDemoName;
    if (!RegisterClass(&Winclass)) return 0;

    RECT Rect = { 0, 0, (i32)m_Resolution[0], (i32)m_Resolution[1] };
    if (!AdjustWindowRect(&Rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, FALSE)) return 0;

    Demo->Window = CreateWindowEx(0, kDemoName, kDemoName,
                                  WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
                                  CW_USEDEFAULT, CW_USEDEFAULT,
                                  Rect.right - Rect.left, Rect.bottom - Rect.top,
                                  nullptr, nullptr, nullptr, 0);
    if (!Demo->Window) return 0;

 
    IDXGIFactory4 *FactoryDXGI = nullptr;
    COMCHECK(CreateDXGIFactory1(IID_IDXGIFactory4, (void **)&FactoryDXGI));
    COMCHECK(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_ID3D12Device, (void **)&Demo->Gpu));

    D3D12_COMMAND_QUEUE_DESC CmdQueueDesc = {};
    CmdQueueDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    CmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    CmdQueueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    COMCHECK(Demo->Gpu->CreateCommandQueue(&CmdQueueDesc, IID_ID3D12CommandQueue, (void **)&Demo->CmdQueue));

    DXGI_SWAP_CHAIN_DESC SwapchainDesc = {};
    SwapchainDesc.BufferCount       = kNumSwapbuffers;
    SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapchainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapchainDesc.OutputWindow      = Demo->Window;
    SwapchainDesc.SampleDesc.Count  = 1;
    SwapchainDesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    SwapchainDesc.Windowed          = (kDemoFullscreen ? FALSE : TRUE);

    IDXGISwapChain *Swapchain = nullptr;
    COMCHECK(FactoryDXGI->CreateSwapChain(Demo->CmdQueue, &SwapchainDesc, &Swapchain));
    COMCHECK(Swapchain->QueryInterface(IID_IDXGISwapChain3, (void **)&Demo->Swapchain));
    COMRELEASE(Swapchain);
    COMRELEASE(FactoryDXGI);

    Demo->RtvSize = Demo->Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    Demo->CbvSrvUavSize = Demo->Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    D3D12_DESCRIPTOR_HEAP_DESC RtvHeapDesc = {};
    RtvHeapDesc.NumDescriptors = kNumSwapbuffers;
    RtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    RtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    COMCHECK(Demo->Gpu->CreateDescriptorHeap(&RtvHeapDesc, IID_ID3D12DescriptorHeap, (void **)&Demo->RtvHeap));
    Demo->RtvHeapStart = Demo->RtvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle = Demo->RtvHeapStart;

    for (u32 BufferIdx = 0; BufferIdx < kNumSwapbuffers; ++BufferIdx)
    {
        COMCHECK(Demo->Swapchain->GetBuffer(BufferIdx, IID_ID3D12Resource, (void **)&Demo->Swapbuffers[BufferIdx]));

        Demo->Gpu->CreateRenderTargetView(Demo->Swapbuffers[BufferIdx], nullptr, RtvHandle);
        RtvHandle.ptr += Demo->RtvSize;
    }

    Demo->Viewport = { 0.0f, 0.0f, (f32)Demo->Resolution[0], (f32)Demo->Resolution[1], 0.0f, 1.0f };
    Demo->ScissorRect = { 0, 0, (i32)Demo->Resolution[0], (i32)Demo->Resolution[1] };


    for (u32 FrameIdx = 0; FrameIdx < kNumBufferedFrames; ++FrameIdx)
    {
        frame_resources *Fres = &Demo->FrameResources[FrameIdx];

        // command allocator
        COMCHECK(Demo->Gpu->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                   IID_ID3D12CommandAllocator, (void **)&Fres->CmdAlloc));


        // GPU visible decriptor heap
        D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
        HeapDesc.NumDescriptors = kNumGpuDescriptors;
        HeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        HeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        COMCHECK(Demo->Gpu->CreateDescriptorHeap(&HeapDesc, IID_ID3D12DescriptorHeap,
                                                 (void **)&Fres->Heap));

        Fres->HeapCpuStart = Fres->Heap->GetCPUDescriptorHandleForHeapStart();
        Fres->HeapGpuStart = Fres->Heap->GetGPUDescriptorHandleForHeapStart();


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
        COMCHECK(Demo->Gpu->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                                    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                    IID_ID3D12Resource, (void **)&Fres->Cb));
    }

    Demo->FrameSync.Value = 0;
    COMCHECK(Demo->Gpu->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_ID3D12Fence,
                                    (void **)&Demo->FrameSync.Fence));
    Demo->FrameSync.Event = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    Assert(Demo->FrameSync.Event);


    COMCHECK(Demo->Gpu->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Demo->FrameResources[0].CmdAlloc,
                                          nullptr, IID_ID3D12GraphicsCommandList, (void **)&Demo->CmdList));
    return 1;
}

static void
Shutdown(demo_state *Demo)
{
    if (Demo->CmdQueue && Demo->FrameSync.Fence)
    {
        //WaitForGpu(Demo->CmdQueue, &Demo->FrameSync);
    }

    //ShutdownDemo(Demo);

    COMRELEASE(Demo->Swapchain);
    COMRELEASE(Demo->CmdQueue);
    COMRELEASE(Demo->Gpu);
}

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

static void
Run(demo_state *Demo)
{
    //UpdateDemo(Demo);

    Demo->Swapchain->Present(0, 0);

    frame_sync *Sync = &Demo->FrameSync;

    const u64 CpuValue = ++Sync->Value;
    Demo->CmdQueue->Signal(Sync->Fence, CpuValue);

    const u64 GpuValue = Sync->Fence->GetCompletedValue();

    if ((CpuValue - GpuValue) >= kNumBufferedFrames)
    {
        Sync->Fence->SetEventOnCompletion(GpuValue + 1, Sync->Event);
        WaitForSingleObject(Sync->Event, INFINITE);
    }

    Demo->SwapbufferIndex = Demo->Swapchain->GetCurrentBackBufferIndex();
    Demo->FrameIndex = ++Demo->FrameIndex % kNumBufferedFrames;
}

void
Start()
{
    s_Kernel32 = LoadLibraryA("kernel32.dll");
    s_User32 = LoadLibraryA("user32.dll");
    s_Gdi32 = LoadLibraryA("gdi32.dll");
    s_Dxgi = LoadLibraryA("dxgi.dll");
    s_D3D12 = LoadLibraryA("d3d12.dll");

    OutputDebugString = (OutputDebugString_fn)GetProcAddress(s_Kernel32, "OutputDebugStringA");
    ExitProcess = (ExitProcess_fn)GetProcAddress(s_Kernel32, "ExitProcess");
    GetModuleHandle = (GetModuleHandle_fn)GetProcAddress(s_Kernel32, "GetModuleHandleA");
    Sleep = (Sleep_fn)GetProcAddress(s_Kernel32, "Sleep");
    CreateEventEx = (CreateEventEx_fn)GetProcAddress(s_Kernel32, "CreateEventExA");
    WaitForSingleObject = (WaitForSingleObject_fn)GetProcAddress(s_Kernel32, "WaitForSingleObject");
    QueryPerformanceCounter = (QueryPerformanceCounter_fn)GetProcAddress(s_Kernel32, "QueryPerformanceCounter");
    QueryPerformanceFrequency = (QueryPerformanceFrequency_fn)GetProcAddress(s_Kernel32, "QueryPerformanceFrequency");

    PeekMessage = (PeekMessage_fn)GetProcAddress(s_User32, "PeekMessageA");
    DispatchMessage = (DispatchMessage_fn)GetProcAddress(s_User32, "DispatchMessageA");
    PostQuitMessage = (PostQuitMessage_fn)GetProcAddress(s_User32, "PostQuitMessage");
    DefWindowProc = (DefWindowProc_fn)GetProcAddress(s_User32, "DefWindowProcA");
    LoadCursor = (LoadCursor_fn)GetProcAddress(s_User32, "LoadCursorA");
    RegisterClass = (RegisterClass_fn)GetProcAddress(s_User32, "RegisterClassA");
    CreateWindowEx = (CreateWindowEx_fn)GetProcAddress(s_User32, "CreateWindowExA");
    AdjustWindowRect = (AdjustWindowRect_fn)GetProcAddress(s_User32, "AdjustWindowRect");
    wsprintf = (wsprintf_fn)GetProcAddress(s_User32, "wsprintfA");
    SetWindowText = (SetWindowText_fn)GetProcAddress(s_User32, "SetWindowTextA");

    CreateDXGIFactory1 = (CreateDXGIFactory1_fn)GetProcAddress(s_Dxgi, "CreateDXGIFactory1");

    D3D12GetDebugInterface = (D3D12GetDebugInterface_fn)GetProcAddress(s_D3D12, "D3D12GetDebugInterface");
    D3D12CreateDevice = (D3D12CreateDevice_fn)GetProcAddress(s_D3D12, "D3D12CreateDevice");

#ifdef _DEBUG
    ID3D12Debug *Dbg = nullptr;
    D3D12GetDebugInterface(IID_ID3D12Debug, (void **)&Dbg);
    if (Dbg)
    {
        Dbg->EnableDebugLayer();
        COMRELEASE(Dbg);
    }
#endif

    demo_state Demo = {};
    Demo.Resolution[0] = kDemoResX;
    Demo.Resolution[1] = kDemoResY;

    if (!Initialize(&Demo))
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
            UpdateFrameStats(Demo.Window, &Demo.Time, &Demo.TimeDelta);
        }
    }

    ExitProcess(0);
}
