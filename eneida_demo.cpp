static bool
CompileShaders(demo *Demo)
{
    size_t VsBytecodeSize;
    size_t PsBytecodeSize;
    void *VsBytecode = LoadFile("data/vs_full_triangle.cso", &VsBytecodeSize);
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
    HRESULT Hr = Demo->Device->CreateGraphicsPipelineState(&PsoDesc, IID_PPV_ARGS(&Demo->TestPso));

    if (VsBytecode) MemFree(VsBytecode);
    if (PsBytecode) MemFree(PsBytecode);
    if (FAILED(Hr)) return false;

    return true;
}

static void
GenerateGpuCommands(demo *Demo)
{
    frame_resources *FrameRes = &Demo->FrameRes[Demo->FrameIndex];
    ID3D12GraphicsCommandList *CmdList = Demo->CmdList;

    FrameRes->CmdAlloc->Reset();

    CmdList->Reset(FrameRes->CmdAlloc, Demo->TestPso);
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

static bool
InitDemo(demo *Demo)
{
    return true;
}

static void
DeinitDemo(demo *Demo)
{
}

static void
UpdateDemo(demo *Demo)
{
    GenerateGpuCommands(Demo);
    Demo->CmdQueue->ExecuteCommandLists(1, (ID3D12CommandList **)&Demo->CmdList);
}
