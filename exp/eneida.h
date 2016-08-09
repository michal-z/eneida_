#include "eneida_windows.h"

// TODO: Finish Assert implementation
#ifdef _DEBUG
#define Assert(Expression) if (!(Expression)) { __debugbreak(); }
#else
#define Assert(Expression)
#endif

#define COMRELEASE(comobj) if ((comobj)) { (comobj)->Release(); (comobj) = nullptr; }
#define COMCHECK(r) if ((r) != 0) { __debugbreak(); }

#define kDemoName "eneida"
#define kDemoResX 1280 
#define kDemoResY 720
#define kDemoFullscreen 0

#define kNumSwapbuffers 4
#define kNumBufferedFrames 3
#define kNumGpuDescriptors 1000

#include "eneida_memory.h"

class FrameResources
{
public:
    i32 Initialize(DemoState *demo_state);

    ID3D12CommandAllocator*     m_CmdAlloc;
    ID3D12Resource*             m_Cb;
    void*                       m_CbCpuAddr;
    D3D12_GPU_VIRTUAL_ADDRESS   m_CbGpuAddr;
    ID3D12DescriptorHeap*       m_Heap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_HeapCpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE m_HeapGpuStart;
};

struct frame_sync
{
    ID3D12Fence *Fence;
    u64         Value;
    void        *Event;
};

class DemoState
{
public:
    u32 SwapbufferIndex;
    u32 FrameIndex;
    u32 Resolution[2];
    f64 Time;
    f32 TimeDelta;

    void *Window;

    ID3D12Device              *Gpu;
    ID3D12CommandQueue        *CmdQueue;
    ID3D12GraphicsCommandList *CmdList;
    IDXGISwapChain3           *Swapchain;

    u32 RtvSize;
    u32 CbvSrvUavSize;

    ID3D12Resource *Swapbuffers[kNumSwapbuffers];

    D3D12_VIEWPORT Viewport;
    D3D12_RECT     ScissorRect;

    ID3D12DescriptorHeap        *RtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE RtvHeapStart;

    frame_sync      FrameSync;
    frame_resources FrameResources[kNumBufferedFrames];
};
