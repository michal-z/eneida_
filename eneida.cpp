#include "eneida.h"
#include "eneida_lib.cpp"
#include "eneida_demo.cpp"

// needed by VC when CRT is not used (/NODEFAULTLIBS)
extern "C" { int _fltused; }

static LRESULT CALLBACK
Winproc(HWND Win, UINT Msg, WPARAM Wparam, LPARAM Lparam)
{
  switch (Msg) {
      case WM_DESTROY:
      case WM_KEYDOWN:
          PostQuitMessage(0);
          return 0;
  }
  return DefWindowProc(Win, Msg, Wparam, Lparam);
}

static bool
Init(demo *Demo)
{
    WNDCLASS Winclass = {};
    Winclass.lpfnWndProc = Winproc;
    Winclass.hInstance = GetModuleHandle(nullptr);
    Winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    Winclass.lpszClassName = kDemoName;
    if (!RegisterClass(&Winclass)) return false;

    if (kDemoFullscreen)
    {
        Demo->Resolution[0] = GetSystemMetrics(SM_CXSCREEN);
        Demo->Resolution[1] = GetSystemMetrics(SM_CYSCREEN);
        ShowCursor(FALSE);
    }

    RECT Rect = { 0, 0, (long)Demo->Resolution[0], (long)Demo->Resolution[1] };
    if (!AdjustWindowRect(&Rect, WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, FALSE))
        return false;

    Demo->Hwnd = CreateWindow(kDemoName, kDemoName,
                              WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              Rect.right - Rect.left, Rect.bottom - Rect.top,
                              nullptr, nullptr, nullptr, 0);
    if (!Demo->Hwnd) return false;

    HRESULT Hr = CreateDXGIFactory1(IID_PPV_ARGS(&Demo->FactoryDxgi));
    if (FAILED(Hr)) return false;

#ifdef _DEBUG
    ID3D12Debug *Dbg = nullptr;
    D3D12GetDebugInterface(IID_PPV_ARGS(&Dbg));
    if (Dbg)
    {
        Dbg->EnableDebugLayer();
        SAFE_RELEASE(Dbg);
    }
#endif

    Hr = D3D12CreateDevice(0, D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&Demo->Device));
    if (FAILED(Hr)) return false;

    D3D12_COMMAND_QUEUE_DESC CmdQueueDesc = {};
    CmdQueueDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;
    CmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    CmdQueueDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;
    Hr = Demo->Device->CreateCommandQueue(&CmdQueueDesc, IID_PPV_ARGS(&Demo->CmdQueue));
    if (FAILED(Hr)) return false;


    DXGI_SWAP_CHAIN_DESC SwapchainDesc = {};
    SwapchainDesc.BufferCount       = kNumSwapbuffers;
    SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapchainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapchainDesc.OutputWindow      = Demo->Hwnd;
    SwapchainDesc.SampleDesc.Count  = 1;
    SwapchainDesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    SwapchainDesc.Windowed          = (kDemoFullscreen ? FALSE : TRUE);
    IDXGISwapChain *Swapchain;
    Hr = Demo->FactoryDxgi->CreateSwapChain(Demo->CmdQueue, &SwapchainDesc, &Swapchain);
    if (FAILED(Hr)) return false;

    Hr = Swapchain->QueryInterface(IID_PPV_ARGS(&Demo->Swapchain));
    SAFE_RELEASE(Swapchain);
    if (FAILED(Hr)) return false;


    Demo->RtvSize = Demo->Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    Demo->CbvSrvUavSize = Demo->Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    D3D12_DESCRIPTOR_HEAP_DESC RtvHeapDesc = {};
    RtvHeapDesc.NumDescriptors = kNumSwapbuffers;
    RtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    RtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Hr = Demo->Device->CreateDescriptorHeap(&RtvHeapDesc, IID_PPV_ARGS(&Demo->RtvHeap));
    if (FAILED(Hr)) return false;

    Demo->RtvHeapStart = Demo->RtvHeap->GetCPUDescriptorHandleForHeapStart();


    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle = Demo->RtvHeapStart;

    for (uint32_t Idx = 0; Idx < kNumSwapbuffers; ++Idx)
    {
        Hr = Demo->Swapchain->GetBuffer(Idx, IID_PPV_ARGS(&Demo->Swapbuffers[Idx]));
        if (FAILED(Hr)) return false;

        Demo->Device->CreateRenderTargetView(Demo->Swapbuffers[Idx], nullptr, RtvHandle);
        RtvHandle.ptr += Demo->RtvSize;
    }

    Demo->Viewport = { 0.0f, 0.0f, (float)Demo->Resolution[0], (float)Demo->Resolution[1], 0.0f, 1.0f };
    Demo->ScissorRect = { 0, 0, (long)Demo->Resolution[0], (long)Demo->Resolution[1] };


    for (uint32_t Idx = 0; Idx < kNumBufferedFrames; ++Idx)
    {
        frame_resources *Fres = &Demo->FrameRes[Idx];

        // command allocator
        Hr = Demo->Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                  IID_PPV_ARGS(&Fres->CmdAlloc));
        if (FAILED(Hr)) return false;


        // GPU visible decriptor heap
        D3D12_DESCRIPTOR_HEAP_DESC HeapDesc = {};
        HeapDesc.NumDescriptors = kNumGpuDescriptors;
        HeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        HeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        Hr = Demo->Device->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&Fres->Heap));
        if (FAILED(Hr)) return false;

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
        Hr = Demo->Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                                   D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                   IID_PPV_ARGS(&Fres->Cb));
        if (FAILED(Hr)) return false;
    }

    Demo->FrameSync.Value = 0;
    Hr = Demo->Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Demo->FrameSync.Fence));
    if (FAILED(Hr)) return false;

    Demo->FrameSync.Event = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    if (!Demo->FrameSync.Event) return false;


    Hr = Demo->Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Demo->FrameRes[0].CmdAlloc,
                                         nullptr, IID_PPV_ARGS(&Demo->CmdList));
    if (FAILED(Hr)) return false;

    if (!InitDemo(Demo)) return false;

    Demo->CmdList->Close();
    Demo->CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList **)&Demo->CmdList);
    WaitForGpu(Demo->CmdQueue, &Demo->FrameSync);

    return true;
}

static void
Deinit(demo *Demo)
{
    if (Demo->CmdQueue && Demo->FrameSync.Fence)
    {
        WaitForGpu(Demo->CmdQueue, &Demo->FrameSync);
    }

    DeinitDemo(Demo);

    SAFE_RELEASE(Demo->Swapchain);
    SAFE_RELEASE(Demo->CmdQueue);
    SAFE_RELEASE(Demo->Device);
    SAFE_RELEASE(Demo->FactoryDxgi);
}

static void
Update(demo *Demo)
{
    UpdateDemo(Demo);

    Demo->Swapchain->Present(0, 0);

    frame_sync *Sync = &Demo->FrameSync;

    const uint64_t CpuValue = ++Sync->Value;
    Demo->CmdQueue->Signal(Sync->Fence, CpuValue);

    const uint64_t GpuValue = Sync->Fence->GetCompletedValue();

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
    demo Demo = {};
    Demo.Resolution[0] = kDemoResX;
    Demo.Resolution[1] = kDemoResY;

    if (!Init(&Demo))
    {
        Deinit(&Demo);
        ExitProcess(1);
    }

    MSG Msg = {};
    for (;;)
    {
        if (PeekMessage(&Msg, 0, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Msg);
            if (Msg.message == WM_QUIT) break;
        }
        else
        {
            UpdateFrameStats(Demo.Hwnd, &Demo.Time, &Demo.TimeDelta);
            Update(&Demo);
        }
    }

    Deinit(&Demo);
    ExitProcess(0);
}
