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

class Demo;

class FrameResources
{
public:
    i32 Initialize(Demo* demo);

    ID3D12CommandAllocator*     m_CmdAlloc;
    ID3D12Resource*             m_Cb;
    void*                       m_CbCpuAddr;
    D3D12_GPU_VIRTUAL_ADDRESS   m_CbGpuAddr;
    ID3D12DescriptorHeap*       m_Heap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_HeapCpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE m_HeapGpuStart;
};

class Demo
{
public:
    i32  Initialize();
    void Shutdown();
    void WaitForGpu();

    u32                         m_FrameIndex;
    u32                         m_Resolution[2];
    f64                         m_Time;
    f32                         m_TimeDelta;
    ID3D12Device*               m_Gpu;
    ID3D12CommandQueue*         m_CmdQueue;
    ID3D12GraphicsCommandList*  m_CmdList;
    u32                         m_RtvSize;
    u32                         m_CbvSrvUavSize;
    D3D12_VIEWPORT              m_Viewport;
    D3D12_RECT                  m_ScissorRect;
    ID3D12DescriptorHeap*       m_RtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RtvHeapStart;
    FrameResources              m_FrameResources[kNumBufferedFrames];

private:
    IDXGISwapChain3*            m_Swapchain;
    void*                       m_Window;
    u32                         m_SwapbufferIndex;
    ID3D12Resource*             m_Swapbuffers[kNumSwapbuffers];
    ID3D12Fence*                m_FrameFence;
    u64                         m_FrameFenceValue;
    void*                       m_FrameFenceEvent;
    void*                       m_Kernel32;
    void*                       m_User32;
    void*                       m_Gdi32;
    void*                       m_Dxgi;
    void*                       m_D3D12;
};
