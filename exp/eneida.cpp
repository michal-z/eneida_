#include "eneida.h"

// needed by VC when CRT is not used (/NODEFAULTLIBS)
extern "C" { i32 _fltused; }

static void *s_Kernel32;
static void *s_User32;
static void *s_Gdi32;
static void *s_Dxgi;
static void *s_D3D12;

static i64 STDCALL
Winproc(void *Window, u32 Message, u64 Param1, i64 Param2)
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

static i32
Init(demo_state *Demo)
{
    WNDCLASS Winclass = {};
    Winclass.lpfnWndProc = Winproc;
    Winclass.hInstance = GetModuleHandle(nullptr);
    Winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    Winclass.lpszClassName = kDemoName;
    if (!RegisterClass(&Winclass)) return 0;

    RECT Rect = { 0, 0, (i32)Demo->Resolution[0], (i32)Demo->Resolution[1] };
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
    COMCHECK(Demo->Gpu->CreateCommandQueue(&CmdQueueDesc, IID_ID3D12CommandQueue,
                                           (void **)&Demo->CmdQueue));

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
    COMCHECK(Demo->Gpu->CreateDescriptorHeap(&RtvHeapDesc, IID_ID3D12DescriptorHeap,
                                             (void **)&Demo->RtvHeap));
    Demo->RtvHeapStart = Demo->RtvHeap->GetCPUDescriptorHandleForHeapStart();

    return 1;
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

    PeekMessage = (PeekMessage_fn)GetProcAddress(s_User32, "PeekMessageA");
    DispatchMessage = (DispatchMessage_fn)GetProcAddress(s_User32, "DispatchMessageA");
    PostQuitMessage = (PostQuitMessage_fn)GetProcAddress(s_User32, "PostQuitMessage");
    DefWindowProc = (DefWindowProc_fn)GetProcAddress(s_User32, "DefWindowProcA");
    LoadCursor = (LoadCursor_fn)GetProcAddress(s_User32, "LoadCursorA");
    RegisterClass = (RegisterClass_fn)GetProcAddress(s_User32, "RegisterClassA");
    CreateWindowEx = (CreateWindowEx_fn)GetProcAddress(s_User32, "CreateWindowExA");
    AdjustWindowRect = (AdjustWindowRect_fn)GetProcAddress(s_User32, "AdjustWindowRect");

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
    Demo.Resolution[0] = 1280;
    Demo.Resolution[1] = 720;

    if (!Init(&Demo))
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
