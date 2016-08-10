static double
GetTime()
{
    static int64_t Freq;
    static int64_t StartCounter;

    if (Freq == 0)
    {
        QueryPerformanceFrequency(&Freq);
        QueryPerformanceCounter(&StartCounter);
    }
    int64_t Counter;
    QueryPerformanceCounter(&Counter);
    return (Counter - StartCounter) / (double)Freq;
}

static void
UpdateFrameStats(void *Win, double *Time, float *TimeDelta)
{
    static double PrevTime = -1.0;
    static double PrevFpsTime = 0.0;
    static uint32_t FpsFrame = 0;

    if (PrevTime < 0.0)
    {
        PrevTime = GetTime();
        PrevFpsTime = PrevTime;
    }

    *Time = GetTime();
    *TimeDelta = (float)(*Time - PrevTime);
    PrevTime = *Time;

    if ((*Time - PrevFpsTime) >= 1.0)
    {
        double Fps = FpsFrame / (*Time - PrevFpsTime);
        double MicroS = (1.0 / Fps) * 1000000.0;
        char Text[256];
        wsprintf(Text, "[%d fps  %d us] %s", (int32_t)Fps, (int32_t)MicroS, kDemoName);
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

static ID3D12Resource *CreateUploadBuffer(ID3D12Device *Gpu, uint64_t BufferSize, void** BufferCpuPtr = nullptr)
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
