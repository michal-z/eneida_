#include "eneida.h"
#include "eneida_lib.cpp"
#include "eneida_test_scene1.cpp"

// needed by VC when CRT is not used (/NODEFAULTLIBS)
extern "C" { int32_t _fltused; }

// internal state visible only in this file
static IDXGISwapChain3* s_Swapchain;
static void*            s_Window;
static ID3D12Fence*     s_FrameFence;
static uint64_t         s_FrameFenceValue;
static void*            s_FrameFenceEvent;
static void*            s_Kernel32;
static void*            s_User32;
static void*            s_Gdi32;
static void*            s_Dxgi;
static void*            s_D3D12;

static int64_t STDCALL WindowsMessageHandler(void *Window, uint32_t Message, uint64_t Param1, int64_t Param2)
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

void CreateFrameResources(uint32_t frame_idx)
{
    Assert(frame_idx < kNumBufferedFrames);

    FrameResources *fr = &G.m_FrameResources[frame_idx];

    // command allocator
    COMCHECK(G.m_Gpu->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                             IID_ID3D12CommandAllocator, (void**)&fr->m_CmdAlloc));

    // GPU visible decriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc = {};
    heap_desc.NumDescriptors = kNumGpuDescriptors;
    heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heap_desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    COMCHECK(G.m_Gpu->CreateDescriptorHeap(&heap_desc, IID_ID3D12DescriptorHeap,
                                         (void**)&fr->m_Heap));

    fr->m_HeapCpuStart = fr->m_Heap->GetCPUDescriptorHandleForHeapStart();
    fr->m_HeapGpuStart = fr->m_Heap->GetGPUDescriptorHandleForHeapStart();


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
    COMCHECK(G.m_Gpu->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                              D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                              IID_ID3D12Resource, (void **)&fr->m_Cb));
}

static int32_t Initialize()
{
#ifdef _DEBUG
    ID3D12Debug *dbg = nullptr;
    D3D12GetDebugInterface(IID_ID3D12Debug, (void **)&dbg);
    if (dbg)
    {
        dbg->EnableDebugLayer();
        COMRELEASE(dbg);
    }
#endif

    G.m_Resolution[0] = kDemoResX;
    G.m_Resolution[1] = kDemoResY;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowsMessageHandler;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = kDemoName;
    if (!RegisterClass(&wc)) {
        Assert(0);
    }

    RECT rect = { 0, 0, (int32_t)G.m_Resolution[0], (int32_t)G.m_Resolution[1] };
    if (!AdjustWindowRect(&rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, FALSE)) return 0;

    s_Window = CreateWindowEx(0, kDemoName, kDemoName,
                                WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                rect.right - rect.left, rect.bottom - rect.top,
                                nullptr, nullptr, nullptr, 0);
    Assert(s_Window);

 
    IDXGIFactory4* factory_dxgi = nullptr;
    COMCHECK(CreateDXGIFactory1(IID_IDXGIFactory4, (void**)&factory_dxgi));
    COMCHECK(D3D12CreateDevice(0, D3D_FEATURE_LEVEL_12_0, IID_ID3D12Device, (void**)&G.m_Gpu));

    D3D12_COMMAND_QUEUE_DESC cmd_queue_desc = {};
    cmd_queue_desc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    cmd_queue_desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    cmd_queue_desc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    COMCHECK(G.m_Gpu->CreateCommandQueue(&cmd_queue_desc, IID_ID3D12CommandQueue, (void**)&G.m_CmdQueue));

    DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
    swapchain_desc.BufferCount       = kNumSwapbuffers;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow      = s_Window;
    swapchain_desc.SampleDesc.Count  = 1;
    swapchain_desc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapchain_desc.Windowed          = (kDemoFullscreen ? FALSE : TRUE);

    IDXGISwapChain *swapchain = nullptr;
    COMCHECK(factory_dxgi->CreateSwapChain(G.m_CmdQueue, &swapchain_desc, &swapchain));
    COMCHECK(swapchain->QueryInterface(IID_IDXGISwapChain3, (void**)&s_Swapchain));
    COMRELEASE(swapchain);
    COMRELEASE(factory_dxgi);

    G.m_RtvSize = G.m_Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    G.m_CbvSrvUavSize = G.m_Gpu->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
    rtv_heap_desc.NumDescriptors = kNumSwapbuffers;
    rtv_heap_desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    COMCHECK(G.m_Gpu->CreateDescriptorHeap(&rtv_heap_desc, IID_ID3D12DescriptorHeap, (void**)&G.m_RtvHeap));
    G.m_RtvHeapStart = G.m_RtvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = G.m_RtvHeapStart;

    for (uint32_t i = 0; i < kNumSwapbuffers; ++i)
    {
        COMCHECK(s_Swapchain->GetBuffer(i, IID_ID3D12Resource, (void**)&G.m_Swapbuffers[i]));

        G.m_Gpu->CreateRenderTargetView(G.m_Swapbuffers[i], nullptr, rtv_handle);
        rtv_handle.ptr += G.m_RtvSize;
    }

    G.m_Viewport = { 0.0f, 0.0f, (float)G.m_Resolution[0], (float)G.m_Resolution[1], 0.0f, 1.0f };
    G.m_ScissorRect = { 0, 0, (int32_t)G.m_Resolution[0], (int32_t)G.m_Resolution[1] };


    for (uint32_t i = 0; i < kNumBufferedFrames; ++i)
    {
        CreateFrameResources(i);
    }

    s_FrameFenceValue = 0;
    COMCHECK(G.m_Gpu->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_ID3D12Fence, (void**)&s_FrameFence));
    s_FrameFenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    Assert(s_FrameFenceEvent);


    COMCHECK(G.m_Gpu->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, G.m_FrameResources[0].m_CmdAlloc,
                                        nullptr, IID_ID3D12GraphicsCommandList, (void**)&G.m_CmdList));
    return 1;
}

static void Shutdown()
{
    COMRELEASE(s_Swapchain);
    COMRELEASE(G.m_CmdQueue);
    COMRELEASE(G.m_Gpu);
}

static void FlushGpu()
{
    if (G.m_CmdQueue && s_FrameFence)
    {
        G.m_CmdQueue->Signal(s_FrameFence, ++s_FrameFenceValue);

        if (s_FrameFence->GetCompletedValue() < s_FrameFenceValue)
        {
            s_FrameFence->SetEventOnCompletion(s_FrameFenceValue, s_FrameFenceEvent);
            WaitForSingleObject(s_FrameFenceEvent, INFINITE);
        }
    }
}

static int32_t Run()
{
    if (!Initialize())
    {
        Shutdown();
        return 1; // error code
    }


    G.m_TemporaryMemory.BeginTempAllocations();

    ResourceSListNode upload_buffers = {};

    TestScene1 scene = {};
    if (!scene.Initialize(&upload_buffers))
    {
        scene.Shutdown();
        Shutdown();
        return 1; // error code
    }
    G.m_CmdList->Close();
    G.m_CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&G.m_CmdList);
    FlushGpu();

    for (ResourceSListNode* n = upload_buffers.m_Next; n; n = n->m_Next)
    {
        COMRELEASE(n->m_Resource);
    }

    G.m_TemporaryMemory.EndTempAllocations();


    for (;;)
    {
        MSG Message = {};
        while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Message);
            if (Message.message == WM_QUIT)
            {
                scene.Shutdown();
                Shutdown();
                return 0; // success code
            }
        }

        UpdateFrameStats(s_Window, &G.m_Time, &G.m_TimeDelta);

        scene.Update();

        s_Swapchain->Present(0, 0);

        const uint64_t CpuValue = ++s_FrameFenceValue;
        G.m_CmdQueue->Signal(s_FrameFence, CpuValue);

        const uint64_t GpuValue = s_FrameFence->GetCompletedValue();

        if ((CpuValue - GpuValue) >= kNumBufferedFrames)
        {
            s_FrameFence->SetEventOnCompletion(GpuValue + 1, s_FrameFenceEvent);
            WaitForSingleObject(s_FrameFenceEvent, INFINITE);
        }

        G.m_SwapbufferIndex = s_Swapchain->GetCurrentBackBufferIndex();
        G.m_FrameIndex = ++G.m_FrameIndex % kNumBufferedFrames;
    }
}

void Start()
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
    VirtualAlloc = (VirtualAlloc_fn)GetProcAddress(s_Kernel32, "VirtualAlloc");
    VirtualFree = (VirtualFree_fn)GetProcAddress(s_Kernel32, "VirtualFree");

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


    uint8_t* memory = (uint8_t*)VirtualAlloc(nullptr, kPersistentMemorySize + kTemporaryMemorySize,
                                             MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!memory)
    {
        // TODO: Add messeage box
        ExitProcess(1);
    }

    G.m_PersistentMemory.Initialize(kPersistentMemorySize, memory);
    G.m_TemporaryMemory.Initialize(kTemporaryMemorySize, memory + kPersistentMemorySize);

    const int32_t exit_code = Run();

    VirtualFree(memory, 0, MEM_RELEASE);
    ExitProcess(exit_code);
}
