#include "eneida_sys.h"

// TODO: Finish Assert implementation
#ifdef _DEBUG
#define Assert(Expression) if (!(Expression)) { __debugbreak(); }
#else
#define Assert(Expression)
#endif

#define COMRELEASE(comobj) { if ((comobj)) { (comobj)->Release(); (comobj) = nullptr; } }
#define COMCHECK(r) { if (r != 0) { Assert(0); } }

#define kDemoName "eneida"
#define kDemoResX 1280 
#define kDemoResY 720
#define kDemoFullscreen 0

#define kNumSwapbuffers 4
#define kNumBufferedFrames 3
#define kNumGpuDescriptors 1000

struct demo_state
{
    u32 SwapbufferIndex;
    u32 FrameIndex;
    u32 Resolution[2];
    f64 Time;
    f32 TimeDelta;

    void *Window;

    ID3D12Device       *Gpu;
    ID3D12CommandQueue *CmdQueue;
    IDXGISwapChain3    *Swapchain;

    u32 RtvSize;
    u32 CbvSrvUavSize;

    ID3D12Resource *Swapbuffers[kNumSwapbuffers];

    D3D12_VIEWPORT Viewport;
    D3D12_RECT     ScissorRect;

    ID3D12DescriptorHeap        *RtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE RtvHeapStart;
};
