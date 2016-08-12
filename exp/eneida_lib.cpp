static double GetTime()
{
    static int64_t freq;
    static int64_t start_counter;

    if (freq == 0)
    {
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start_counter);
    }
    int64_t counter;
    QueryPerformanceCounter(&counter);
    return (counter - start_counter) / (double)freq;
}

static void UpdateFrameStats(void* win, double* time, float* time_delta)
{
    static double prev_time = -1.0;
    static double prev_fps_time = 0.0;
    static uint32_t fps_frame = 0;

    if (prev_time < 0.0)
    {
        prev_time = GetTime();
        prev_fps_time = prev_time;
    }

    *time = GetTime();
    *time_delta = (float)(*time - prev_time);
    prev_time = *time;

    if ((*time - prev_fps_time) >= 1.0)
    {
        double fps = fps_frame / (*time - prev_fps_time);
        double micro_sec = (1.0 / fps) * 1000000.0;
        char text[256];
        wsprintf(text, "[%d fps  %d us] %s", (int32_t)fps, (int32_t)micro_sec, kDemoName);
        SetWindowText(win, text);
        prev_fps_time = *time;
        fps_frame = 0;
    }
    fps_frame++;
}

static void TransitionBarrier(ID3D12GraphicsCommandList* cmdlist, ID3D12Resource* resource,
                              D3D12_RESOURCE_STATES state_before, D3D12_RESOURCE_STATES state_after)
{
    D3D12_RESOURCE_BARRIER barrier_desc = {};
    barrier_desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier_desc.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier_desc.Transition.pResource   = resource;
    barrier_desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier_desc.Transition.StateBefore = state_before;
    barrier_desc.Transition.StateAfter  = state_after;
    cmdlist->ResourceBarrier(1, &barrier_desc);
}

static ID3D12Resource* CreateUploadBuffer(uint64_t buffer_size, void** buffer_cpu_addr = nullptr)
{
    Assert(buffer_size > 0);

    D3D12_HEAP_PROPERTIES heap_props = {};
    heap_props.Type = D3D12_HEAP_TYPE_UPLOAD;

    D3D12_RESOURCE_DESC buffer_desc = {};
    buffer_desc.Dimension        = D3D12_RESOURCE_DIMENSION_BUFFER;
    buffer_desc.Width            = buffer_size;
    buffer_desc.Height           = 1;
    buffer_desc.DepthOrArraySize = 1;
    buffer_desc.MipLevels        = 1;
    buffer_desc.SampleDesc.Count = 1;
    buffer_desc.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    ID3D12Resource* upload_buffer = nullptr;
    COMCHECK(G.m_Gpu->CreateCommittedResource(&heap_props, D3D12_HEAP_FLAG_NONE, &buffer_desc,
                                              D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                              IID_ID3D12Resource, (void**)&upload_buffer));
    if (buffer_cpu_addr)
    {
        D3D12_RANGE range = {};
        COMCHECK(upload_buffer->Map(0, &range, buffer_cpu_addr));
    }

    return upload_buffer;
}
