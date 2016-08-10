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

static uint32_t                    s_FrameIndex;
static uint32_t                    s_Resolution[2];
static double                      s_Time;
static float                       s_TimeDelta;
static ID3D12Device*               s_Gpu;
static ID3D12CommandQueue*         s_CmdQueue;
static ID3D12GraphicsCommandList*  s_CmdList;
static uint32_t                    s_RtvSize;
static uint32_t                    s_CbvSrvUavSize;
static D3D12_VIEWPORT              s_Viewport;
static D3D12_RECT                  s_ScissorRect;
static ID3D12DescriptorHeap*       s_RtvHeap;
static D3D12_CPU_DESCRIPTOR_HANDLE s_RtvHeapStart;
static struct FrameResources
{
    ID3D12CommandAllocator*     m_CmdAlloc;
    ID3D12Resource*             m_Cb;
    void*                       m_CbCpuAddr;
    D3D12_GPU_VIRTUAL_ADDRESS   m_CbGpuAddr;
    ID3D12DescriptorHeap*       m_Heap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_HeapCpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE m_HeapGpuStart;
}                                  s_FrameResources[kNumBufferedFrames];

static void FlushGpu();
