static bool
CompileShaders(demo_state *Demo)
{
    size_t VsBytecodeSize;
    size_t PsBytecodeSize;
    void *VsBytecode = LoadFile("data/shader/XFormShadeVS.cso", &VsBytecodeSize);
    void *PsBytecode = LoadFile("data/ps_sketch0.cso", &PsBytecodeSize);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC PsoDesc = {};
    PsoDesc.VS                       = { VsBytecode, VsBytecodeSize };
    PsoDesc.PS                       = { PsBytecode, PsBytecodeSize };
    PsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
    PsoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    PsoDesc.BlendState.
        RenderTarget[0].
        RenderTargetWriteMask        = D3D12_COLOR_WRITE_ENABLE_ALL;
    PsoDesc.SampleMask               = UINT_MAX;
    PsoDesc.PrimitiveTopologyType    = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    PsoDesc.NumRenderTargets         = 1;
    PsoDesc.RTVFormats[0]            = DXGI_FORMAT_R8G8B8A8_UNORM;
    PsoDesc.SampleDesc.Count         = 1;
    HRESULT Hr = Demo->Device->CreateGraphicsPipelineState(&PsoDesc,
                                                           IID_PPV_ARGS(&Demo->XFormShadePso));
    if (VsBytecode) UnloadFile(VsBytecode);
    if (PsBytecode) UnloadFile(PsBytecode);
    if (FAILED(Hr)) return false;

    return true;
}

static void
GenerateGpuCommands(demo_state *Demo)
{
    frame_resources *FrameRes = &Demo->FrameRes[Demo->FrameIndex];
    ID3D12GraphicsCommandList *CmdList = Demo->CmdList;

    FrameRes->CmdAlloc->Reset();

    CmdList->Reset(FrameRes->CmdAlloc, Demo->XFormShadePso);
    //cmdlist->SetDescriptorHeaps(1, &fres->dheap);

    CmdList->RSSetViewports(1, &Demo->Viewport);
    CmdList->RSSetScissorRects(1, &Demo->ScissorRect);

    TransitionBarrier(CmdList, Demo->Swapbuffers[Demo->SwapbufferIndex], D3D12_RESOURCE_STATE_PRESENT,
                      D3D12_RESOURCE_STATE_RENDER_TARGET);

    float ClearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };

    D3D12_CPU_DESCRIPTOR_HANDLE RtvHandle = Demo->RtvHeapStart;
    RtvHandle.ptr += Demo->SwapbufferIndex * Demo->RtvSize;

    //cmdlist->SetGraphicsRootSignature(demo->root_signature);
    //cmdlist->SetGraphicsRootDescriptorTable(0, fres->gpu_dheap_start);

    CmdList->ClearRenderTargetView(RtvHandle, ClearColor, 0, nullptr);
    CmdList->OMSetRenderTargets(1, &RtvHandle, TRUE, nullptr);
    //cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //ID3D12GraphicsCommandList_DrawInstanced(demo->cmd_list, 3, 1, 0, 0);

    TransitionBarrier(Demo->CmdList, Demo->Swapbuffers[Demo->SwapbufferIndex],
                      D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

    CmdList->Close();
}

struct DxResourceNode
{
    DxResourceNode *Next;
    ID3D12Resource *Resource;
};

static bool
UploadBuffers(demo_state *Demo, DxResourceNode *UploadResources, temporary_memory Memory)
{
    HRESULT Hr = S_OK;
    mesh *Mesh = &Demo->Mesh;
    void *Addr = nullptr;
    ID3D12Device *Device = Demo->Device;
    ID3D12GraphicsCommandList *CmdList = Demo->CmdList;

    ID3D12Resource *UploadVBuf = CreateUploadBuffer(Demo->Device, 64 * 1024, &Addr);
    float *VAddr = (float *)Addr;
    *VAddr++ = -0.7f; *VAddr++ = -0.7f; *VAddr++ = 0.0f;
    *VAddr++ = 0.7f; *VAddr++ = -0.7f; *VAddr++ = 0.0f;
    *VAddr++ = 0.0f; *VAddr++ = 0.7f; *VAddr++ = 0.0f;

    DxResourceNode *Node0 = (DxResourceNode *)Allocate(Memory.Arena, sizeof(DxResourceNode));
    Node0->Next = UploadResources->Next;
    Node0->Resource = UploadVBuf;
    UploadResources->Next = Node0;


    ID3D12Resource *UploadIBuf = CreateUploadBuffer(Demo->Device, 64 * 1024, &Addr);
    uint32_t *IAddr = (uint32_t *)Addr;
    *IAddr++ = 0; *IAddr++ = 1; *IAddr++ = 2;

    DxResourceNode *Node1 = (DxResourceNode *)Allocate(Memory.Arena, sizeof(DxResourceNode));
    Node1->Next = UploadResources->Next;
    Node1->Resource = UploadIBuf;
    UploadResources->Next = Node1;


    D3D12_HEAP_PROPERTIES HeapProps = {};
    HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC BufferDesc = {};
    BufferDesc.Dimension           = D3D12_RESOURCE_DIMENSION_BUFFER;
    BufferDesc.Height              = 1;
    BufferDesc.DepthOrArraySize    = 1;
    BufferDesc.MipLevels           = 1;
    BufferDesc.SampleDesc.Count    = 1;
    BufferDesc.Layout              = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    BufferDesc.Width               = 64 * 1024;
    Hr = Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                         D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
                                         IID_PPV_ARGS(&Mesh->VBuffer));
    if (FAILED(Hr)) return false;


    Hr = Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &BufferDesc,
                                         D3D12_RESOURCE_STATE_COPY_DEST, nullptr,
                                         IID_PPV_ARGS(&Mesh->IBuffer));
    if (FAILED(Hr)) return false;


    CmdList->CopyBufferRegion(Mesh->VBuffer, 0, UploadVBuf, 0, 3 * 3 * sizeof(float));
    CmdList->CopyBufferRegion(Mesh->IBuffer, 0, UploadIBuf, 0, 3 * sizeof(uint32_t));

    D3D12_RESOURCE_BARRIER Barrier[2] = {};
    Barrier[0].Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    Barrier[0].Transition.pResource   = Mesh->VBuffer;
    Barrier[0].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    Barrier[0].Transition.StateAfter  = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

    Barrier[1].Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    Barrier[1].Transition.pResource   = Mesh->IBuffer;
    Barrier[1].Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    Barrier[1].Transition.StateAfter  = D3D12_RESOURCE_STATE_INDEX_BUFFER;

    CmdList->ResourceBarrier(_countof(Barrier), Barrier);

    Mesh->VBufferView.BufferLocation = Mesh->VBuffer->GetGPUVirtualAddress();
    Mesh->VBufferView.StrideInBytes = 3 * sizeof(float);
    Mesh->VBufferView.SizeInBytes = 3 * 3 * sizeof(float);

    Mesh->IBufferView.BufferLocation = Mesh->IBuffer->GetGPUVirtualAddress();
    Mesh->IBufferView.Format = DXGI_FORMAT_R32_UINT;
    Mesh->IBufferView.SizeInBytes = 3 * sizeof(uint32_t);

    return true;
}

static bool
InitializeDemo(demo_state *Demo, DxResourceNode *UploadResources, temporary_memory Memory)
{
    if (!UploadBuffers(Demo, UploadResources, Memory)) return false;

    return true;
}

static void
ShutdownDemo(demo_state *Demo)
{
}

static void
UpdateDemo(demo_state *Demo)
{
    GenerateGpuCommands(Demo);
    Demo->CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList **)&Demo->CmdList);
}
