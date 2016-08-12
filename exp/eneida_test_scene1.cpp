struct ResourceArray
{
    uint32_t m_Size;
    ID3D12Resource** m_Resources;
};

class TestScene1
{
public:
    int32_t Initialize(uint32_t* num_upload_buffers, ID3D12Resource*** upload_buffers)
    {
        ID3D12Resource* buf0 = CreateUploadBuffer(100);
        ID3D12Resource* buf1 = CreateUploadBuffer(100);

        *upload_buffers = (ID3D12Resource**)G.m_TemporaryMemory.GetAllocationBaseAddr(16);
        G.m_TemporaryMemory.Push(buf0);
        G.m_TemporaryMemory.Push(buf1);
        *num_upload_buffers = 2;

        return 1;
    }
    void Shutdown()
    {
        FlushGpu();
    }
    void Update()
    {
    }

private:
    float m_ClearColor[4];
};
