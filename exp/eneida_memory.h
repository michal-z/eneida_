class MemoryArena
{
public:
    void Initialize(uint64_t size, void* base)
    {
        m_Base = (uint8_t *)base;
        m_Offset = 0;
        m_Size = size;
        m_TempAllocations = 0;
    }

    void* Allocate(uint64_t size, uint64_t alignment = 8)
    {
        uint64_t alignment_offset = GetAlignmentOffset(alignment);
        uint64_t alloc_size = m_Size + alignment_offset;

        Assert((m_Offset + alloc_size) <= m_Size);

        void* result = (void*)((uint64_t)m_Base + m_Offset + alignment_offset);
        m_Offset += alloc_size;

        Assert(alloc_size >= m_Size);

        return result;
    }

    void BeginTempAllocations()
    {
        Assert((m_TempAllocations + 1) < kMaxNestedTempAllocations);

        m_TempOffsets[m_TempAllocations++] = m_Offset;
    }

    void EndTempAllocations()
    {
        Assert(m_TempAllocations > 0);

        m_Offset = m_TempOffsets[--m_TempAllocations];
    }

private:
    uint64_t GetAlignmentOffset(uint64_t alignment)
    {
        uint64_t current_addr = (uint64_t)m_Base + m_Offset;
        uint64_t alignment_offset = 0;

        if (current_addr & (alignment - 1))
        {
            alignment_offset = alignment - (current_addr & (alignment - 1));
        }

        return alignment_offset;
    }

    static const int32_t kMaxNestedTempAllocations = 16;

    int32_t  m_TempAllocations;
    uint64_t m_TempOffsets[kMaxNestedTempAllocations];
    uint64_t m_Offset;
    uint8_t* m_Base;
    uint64_t m_Size;
};
