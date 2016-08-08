#include "eneida_sys.h"

#define kDemoName "eneida"

struct demo_state
{
    u32  Resolution[2];
    void *Window;

    IDXGIFactory4      *FactoryDXGI;
    ID3D12Device       *Gpu;
    ID3D12CommandQueue *CmdQueue;
};
