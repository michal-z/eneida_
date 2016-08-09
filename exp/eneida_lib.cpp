static f64
GetTime()
{
    static i64 Freq;
    static i64 StartCounter;

    if (Freq == 0)
    {
        QueryPerformanceFrequency(&Freq);
        QueryPerformanceCounter(&StartCounter);
    }
    i64 Counter;
    QueryPerformanceCounter(&Counter);
    return (Counter - StartCounter) / (f64)Freq;
}

static void
UpdateFrameStats(void *Win, f64 *Time, f32 *TimeDelta)
{
    static f64 PrevTime = -1.0;
    static f64 PrevFpsTime = 0.0;
    static u32 FpsFrame = 0;

    if (PrevTime < 0.0)
    {
        PrevTime = GetTime();
        PrevFpsTime = PrevTime;
    }

    *Time = GetTime();
    *TimeDelta = (f32)(*Time - PrevTime);
    PrevTime = *Time;

    if ((*Time - PrevFpsTime) >= 1.0)
    {
        f64 Fps = FpsFrame / (*Time - PrevFpsTime);
        f64 MicroS = (1.0 / Fps) * 1000000.0;
        char Text[256];
        wsprintf(Text, "[%d fps  %d us] %s", (i32)Fps, (i32)MicroS, kDemoName);
        SetWindowText(Win, Text);
        PrevFpsTime = *Time;
        FpsFrame = 0;
    }
    FpsFrame++;
}

static void
TransitionBarrier(ID3D12GraphicsCommandList *CmdList, ID3D12Resource *Resource,
                  D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter)
{
    D3D12_RESOURCE_BARRIER BarrierDesc = {};
    BarrierDesc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    BarrierDesc.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    BarrierDesc.Transition.pResource   = Resource;
    BarrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    BarrierDesc.Transition.StateBefore = StateBefore;
    BarrierDesc.Transition.StateAfter  = StateAfter;
    CmdList->ResourceBarrier(1, &BarrierDesc);
}

static void
WaitForGpu(ID3D12CommandQueue *CmdQueue, frame_sync *F)
{
    F->Value++;

    CmdQueue->Signal(F->Fence, F->Value);

    if (F->Fence->GetCompletedValue() < F->Value)
    {
        F->Fence->SetEventOnCompletion(F->Value, F->Event);
        WaitForSingleObject(F->Event, INFINITE);
    }
}

static ID3D12Resource *
CreateUploadBuffer(ID3D12Device *Gpu, u64 BufferSize, void **BufferCpuPtr = nullptr)
{
    Assert(BufferSize > 0);

    D3D12_HEAP_PROPERTIES HeapProps = {};
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC BufferDesc = {};
    BufferDesc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
    BufferDesc.Width            = BufferSize;
    BufferDesc.Height           = 1;
    BufferDesc.DepthOrArraySize = 1;
    BufferDesc.MipLevels        = 1;
    BufferDesc.SampleDesc.Count = 1;
    BufferDesc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource *UploadBuffer = nullptr;
    COMCHECK(Gpu->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                          D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                          IID_ID3D12Resource, (void **)&UploadBuffer));
    if (BufferCpuPtr)
    {
        D3D12_RANGE range = {};
        COMCHECK(UploadBuffer->Map(0, &range, BufferCpuPtr));
    }

    return UploadBuffer;
}
