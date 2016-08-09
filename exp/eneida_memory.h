struct memory_arena
{
    u8  *Base;
    u64 Offset;
    u64 Size;
    i32 TempAllocations;
};

struct temporary_memory
{
    memory_arena *Arena;
    u64          Offset;
};

inline void
InitializeArena(memory_arena *Arena, u64 Size, void *Base)
{
    Arena->Base = (u8 *)Base;
    Arena->Offset = 0;
    Arena->Size = Size;
    Arena->TempAllocations = 0;
}

inline u64
GetAlignmentOffset(memory_arena *Arena, u64 Alignment)
{
    u64 CurrentAddr = (u64)Arena->Base + Arena->Offset;
    u64 AlignmentOffset = 0;

    if (CurrentAddr & (Alignment - 1))
    {
        AlignmentOffset = Alignment - (CurrentAddr & (Alignment - 1));
    }

    return AlignmentOffset;
}

inline void *
Allocate(memory_arena *Arena, u64 Size, u64 Alignment = 8)
{
    u64 AlignmentOffset = GetAlignmentOffset(Arena, Alignment);
    u64 AllocSize = Size + AlignmentOffset;
    
    Assert((Arena->Offset + AllocSize) <= Arena->Size);

    void *Result = (void *)((u64)Arena->Base + Arena->Offset + AlignmentOffset);
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
