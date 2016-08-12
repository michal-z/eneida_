class TestScene1
{
public:
    int32_t Initialize(ResourceSListNode* upload_buffers)
    {
        ID3D12Resource* buf0 = CreateUploadBuffer(100);
        ID3D12Resource* buf1 = CreateUploadBuffer(100);

        ResourceSListNode* nodes = (ResourceSListNode*)G.m_TemporaryMemory.Allocate(2 * sizeof(ResourceSListNode));

        nodes[0].m_Next = upload_buffers->m_Next;
        nodes[0].m_Resource = buf0;
        upload_buffers->m_Next = &nodes[0];

        nodes[1].m_Next = upload_buffers->m_Next;
        nodes[1].m_Resource = buf1;
        upload_buffers->m_Next = &nodes[1];


        return 1;
    }
    void Shutdown()
    {
        FlushGpu();
    }
    void Update()
    {
        FrameResources* fres = &G.m_FrameResources[G.m_FrameIndex];
        ID3D12GraphicsCommandList* cmdlist = G.m_CmdList;

        fres->m_CmdAlloc->Reset();

        cmdlist->Reset(fres->m_CmdAlloc, nullptr);

        cmdlist->RSSetViewports(1, &G.m_Viewport);
        cmdlist->RSSetScissorRects(1, &G.m_ScissorRect);

        TransitionBarrier(cmdlist, G.m_Swapbuffers[G.m_SwapbufferIndex], D3D12_RESOURCE_STATE_PRESENT,
                          D3D12_RESOURCE_STATE_RENDER_TARGET);


        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = G.m_RtvHeapStart;
        rtv_handle.ptr += G.m_SwapbufferIndex * G.m_RtvSize;

        cmdlist->ClearRenderTargetView(rtv_handle, m_ClearColor, 0, nullptr);
        cmdlist->OMSetRenderTargets(1, &rtv_handle, TRUE, nullptr);


        TransitionBarrier(G.m_CmdList, G.m_Swapbuffers[G.m_SwapbufferIndex],
                          D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

        cmdlist->Close();


        G.m_CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&cmdlist);
    }

private:
    float m_ClearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
};
