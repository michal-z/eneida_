static void
UnloadFile(void *Addr)
{
    Assert(Addr);
    VirtualFree(Addr, 0, MEM_RELEASE);
}

static void *
LoadFile(const char *Filename, size_t *Filesize)
{
    if (!Filename || !Filesize) return nullptr;

    HANDLE File = CreateFile(Filename, GENERIC_READ, 0, nullptr, OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL, nullptr);
    if (File == INVALID_HANDLE_VALUE) return nullptr;

    DWORD Size = GetFileSize(File, nullptr);
    if (Size == INVALID_FILE_SIZE)
    {
        CloseHandle(File);
        return nullptr;
    }

    void *Data = VirtualAlloc(nullptr, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!Data)
    {
        CloseHandle(File);
        return nullptr;
    }

    DWORD Bytes;
    BOOL Res = ReadFile(File, Data, Size, &Bytes, nullptr);
    if (!Res || (Bytes != Size))
    {
        CloseHandle(File);
        UnloadFile(Data);
        return nullptr;
    }

    CloseHandle(File);
    *Filesize = Size;
    return Data;
}

static double
GetTime()
{
    static LARGE_INTEGER Freq = {};
    static LARGE_INTEGER StartCounter = {};

    if (Freq.QuadPart == 0)
    {
        QueryPerformanceFrequency(&Freq);
        QueryPerformanceCounter(&StartCounter);
    }
    LARGE_INTEGER Counter = {};
    QueryPerformanceCounter(&Counter);
    return (Counter.QuadPart - StartCounter.QuadPart) / (double)Freq.QuadPart;
}

static void
UpdateFrameStats(HWND Win, double *Time, float *TimeDelta)
{
    static double PrevTime = -1.0;
    static double PrevFpsTime = 0.0;
    static int FpsFrame = 0;

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
CreateUploadBuffer(ID3D12Device *Device, uint64_t BufferSize, void **BufferCpuPtr = nullptr)
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
    HRESULT Hr = Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                                 D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                 IID_PPV_ARGS(&UploadBuffer));
    if (FAILED(Hr)) return nullptr;

    if (BufferCpuPtr)
    {
        D3D12_RANGE range = {};
        Hr = UploadBuffer->Map(0, &range, BufferCpuPtr);
        if (FAILED(Hr))
        {
            SAFE_RELEASE(UploadBuffer);
            return nullptr;
        }
    }

    return UploadBuffer;
}
