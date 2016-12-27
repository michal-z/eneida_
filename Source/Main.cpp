#include "Main.h"
#include "EASTL/vector.h"


void* operator new[](size_t size, const char* /*name*/, int /*flags*/,
                     unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
    return malloc(size);
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* /*name*/,
                     int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
    return _aligned_offset_malloc(size, alignment, alignmentOffset);
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    eastl::vector<int> x;
    x.push_back(1);

    return 0;
}
