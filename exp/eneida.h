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

#define kPersistentMemorySize 64 * 1024
#define kTemporaryMemorySize 32 * 1024

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

struct ResourceSListNode
{
    ResourceSListNode* m_Next;
    ID3D12Resource*    m_Resource;
};

#include "eneida_memory.h"

// global read-only state
static struct
{
    uint32_t                    m_FrameIndex;
    uint32_t                    m_Resolution[2];
    double                      m_Time;
    float                       m_TimeDelta;
    ID3D12Device*               m_Gpu;
    ID3D12CommandQueue*         m_CmdQueue;
    ID3D12GraphicsCommandList*  m_CmdList;
    uint32_t                    m_RtvSize;
    uint32_t                    m_CbvSrvUavSize;
    D3D12_VIEWPORT              m_Viewport;
    D3D12_RECT                  m_ScissorRect;
    ID3D12DescriptorHeap*       m_RtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RtvHeapStart;
    FrameResources              m_FrameResources[kNumBufferedFrames];
    MemoryArena                 m_PersistentMemory;
    MemoryArena                 m_TemporaryMemory;
} G;

static void FlushGpu();
extern "C" void* memset(void* dest, int32_t value, size_t length); // defined in eneida_asmlib.asm

