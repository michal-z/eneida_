struct memory_arena
{
    uint8_t *Base;
    size_t Offset;
    size_t Size;
    int32_t TempAllocations;
};

struct temporary_memory
{
    memory_arena *Arena;
    size_t Offset;
};

inline void
InitializeArena(memory_arena *Arena, size_t Size, void *Base)
{
    Arena->Base = (uint8_t *)Base;
    Arena->Offset = 0;
    Arena->Size = Size;
    Arena->TempAllocations = 0;
}

inline size_t
GetAlignmentOffset(memory_arena *Arena, size_t Alignment)
{
    size_t CurrentAddr = (size_t)Arena->Base + Arena->Offset;
    size_t AlignmentOffset = 0;

    if (CurrentAddr & (Alignment - 1))
    {
        AlignmentOffset = Alignment - (CurrentAddr & (Alignment - 1));
    }

    return AlignmentOffset;
}

inline void *
Allocate(memory_arena *Arena, size_t Size, size_t Alignment = 8)
{
    size_t AlignmentOffset = GetAlignmentOffset(Arena, Alignment);
    size_t AllocSize = Size + AlignmentOffset;
    
    Assert((Arena->Offset + AllocSize) <= Arena->Size);

    void *Result = (void *)((size_t)Arena->Base + Arena->Offset + AlignmentOffset);
    Arena->Offset += AllocSize;

    Assert(AllocSize >= Size);

    return Result;
}

inline temporary_memory
BeginTemporaryMemory(memory_arena *Arena)
{
    temporary_memory Result;

    Result.Arena = Arena;
    Result.Offset = Arena->Offset;

    Arena->TempAllocations++;

    return Result;
}

inline void
EndTemporaryMemory(temporary_memory TempMem)
{
    memory_arena *Arena = TempMem.Arena;

    Assert(Arena->Offset >= TempMem.Offset);
    Arena->Offset = TempMem.Offset;

    Assert(Arena->TempAllocations > 0);
    Arena->TempAllocations--;
}
