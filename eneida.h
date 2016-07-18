#pragma once

#include <stdint.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <dxgi1_4.h>
#include <d3d12.h>

#define kNumSwapbuffers 4
#define kNumBufferedFrames 3
#define kNumGpuDescriptors 1000

#define kDemoName "eneida"
#define kDemoResX 1024
#define kDemoResY 1024
#define kDemoFullscreen 0

#define SAFE_RELEASE(x) if ((x)) { (x)->Release(); (x) = nullptr; }


struct frame_resources
{
    ID3D12CommandAllocator *CmdAlloc;

    ID3D12Resource *Cb;
    void *CbCpuAddr;
    D3D12_GPU_VIRTUAL_ADDRESS CbGpuAddr;

    ID3D12DescriptorHeap *Heap;
    D3D12_CPU_DESCRIPTOR_HANDLE HeapCpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE HeapGpuStart;
};

struct frame_sync
{
    ID3D12Fence *Fence;
    uint64_t Value;
    HANDLE Event;
};

struct demo
{
    uint32_t SwapbufferIndex;
    uint32_t FrameIndex;
    uint32_t Resolution[2];
    double Time;
    float TimeDelta;

    HWND Hwnd;
    IDXGIFactory4 *FactoryDxgi;
    IDXGISwapChain3 *Swapchain;

    ID3D12Device *Device;
    ID3D12CommandQueue *CmdQueue;
    ID3D12GraphicsCommandList *CmdList;

    uint32_t RtvSize;
    uint32_t CbvSrvUavSize;

    ID3D12Resource *Swapbuffers[kNumSwapbuffers];

    D3D12_VIEWPORT Viewport;
    D3D12_RECT ScissorRect;

    ID3D12DescriptorHeap *RtvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE RtvHeapStart;

    ID3D12PipelineState *TestPso;

    frame_sync FrameSync;
    frame_resources FrameRes[kNumBufferedFrames];
};

static void *MemAlloc(size_t NumBytes);
static void MemFree(void *Addr);
