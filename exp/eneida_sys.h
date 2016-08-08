typedef char                i8;
typedef unsigned char       u8;
typedef short               i16;
typedef unsigned short      u16;
typedef int                 i32;
typedef unsigned int        u32;
typedef long long           i64;
typedef unsigned long long  u64;
typedef float               f32;
typedef double              f64;
typedef int                 b32;
typedef bool                b8;

#define PM_REMOVE           0x0001
#define WM_QUIT             0x0012
#define WM_DESTROY          0x0002
#define WM_KEYDOWN          0x0100
#define WS_OVERLAPPED       0x00000000L
#define WS_VISIBLE          0x10000000L
#define WS_CAPTION          0x00C00000L
#define WS_SYSMENU          0x00080000L
#define WS_MINIMIZEBOX      0x00020000L
#define CW_USEDEFAULT       ((i32)0x80000000)

#define MAKEINTRESOURCE(i) ((char *)((u64)((u16)(i))))

#define IDC_ARROW MAKEINTRESOURCE(32512)

#define STDCALL __stdcall

#define FALSE 0
#define TRUE 1

struct POINT
{
    i32 x;
    i32 y;
};

struct MSG
{
    void  *hwnd;
    u32   message;
    u64   wparam;
    i64   lparam;
    u32   time;
    POINT pt;
};

struct WNDCLASS
{
    u32        style;
    void       *lpfnWndProc;
    i32        cbClsExtra;
    i32        cbWndExtra;
    void       *hInstance;
    void       *hIcon;
    void       *hCursor;
    void       *hbrBackground;
    const char *lpszMenuName;
    const char *lpszClassName;
};

struct RECT
{
    i32 left;
    i32 top;
    i32 right;
    i32 bottom;
};

struct GUID
{
    u32 Data1;
    u16 Data2;
    u16 Data3;
    u8  Data4[8];
};

struct LUID
{
    u32 LowPart;
    i32 HighPart;
};

struct ID3D12Resource;
struct ID3D12RootSignature;

enum DXGI_FORMAT
{
    DXGI_FORMAT_UNKNOWN	                    = 0,
    DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
    DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
    DXGI_FORMAT_R32G32B32A32_UINT           = 3,
    DXGI_FORMAT_R32G32B32A32_SINT           = 4,
    DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
    DXGI_FORMAT_R32G32B32_FLOAT             = 6,
    DXGI_FORMAT_R32G32B32_UINT              = 7,
    DXGI_FORMAT_R32G32B32_SINT              = 8,
    DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
    DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
    DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
    DXGI_FORMAT_R16G16B16A16_UINT           = 12,
    DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
    DXGI_FORMAT_R16G16B16A16_SINT           = 14,
    DXGI_FORMAT_R32G32_TYPELESS             = 15,
    DXGI_FORMAT_R32G32_FLOAT                = 16,
    DXGI_FORMAT_R32G32_UINT                 = 17,
    DXGI_FORMAT_R32G32_SINT                 = 18,
    DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
    DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
    DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
    DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
    DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
    DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
    DXGI_FORMAT_R10G10B10A2_UINT            = 25,
    DXGI_FORMAT_R11G11B10_FLOAT             = 26,
    DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
    DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
    DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
    DXGI_FORMAT_R8G8B8A8_UINT               = 30,
    DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
    DXGI_FORMAT_R8G8B8A8_SINT               = 32,
    DXGI_FORMAT_R16G16_TYPELESS             = 33,
    DXGI_FORMAT_R16G16_FLOAT                = 34,
    DXGI_FORMAT_R16G16_UNORM                = 35,
    DXGI_FORMAT_R16G16_UINT                 = 36,
    DXGI_FORMAT_R16G16_SNORM                = 37,
    DXGI_FORMAT_R16G16_SINT                 = 38,
    DXGI_FORMAT_R32_TYPELESS                = 39,
    DXGI_FORMAT_D32_FLOAT                   = 40,
    DXGI_FORMAT_R32_FLOAT                   = 41,
    DXGI_FORMAT_R32_UINT                    = 42,
    DXGI_FORMAT_R32_SINT                    = 43,
    DXGI_FORMAT_R24G8_TYPELESS              = 44,
    DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
    DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
    DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
    DXGI_FORMAT_R8G8_TYPELESS               = 48,
    DXGI_FORMAT_R8G8_UNORM                  = 49,
    DXGI_FORMAT_R8G8_UINT                   = 50,
    DXGI_FORMAT_R8G8_SNORM                  = 51,
    DXGI_FORMAT_R8G8_SINT                   = 52,
    DXGI_FORMAT_R16_TYPELESS                = 53,
    DXGI_FORMAT_R16_FLOAT                   = 54,
    DXGI_FORMAT_D16_UNORM                   = 55,
    DXGI_FORMAT_R16_UNORM                   = 56,
    DXGI_FORMAT_R16_UINT                    = 57,
    DXGI_FORMAT_R16_SNORM                   = 58,
    DXGI_FORMAT_R16_SINT                    = 59,
    DXGI_FORMAT_R8_TYPELESS                 = 60,
    DXGI_FORMAT_R8_UNORM                    = 61,
    DXGI_FORMAT_R8_UINT                     = 62,
    DXGI_FORMAT_R8_SNORM                    = 63,
    DXGI_FORMAT_R8_SINT                     = 64,
    DXGI_FORMAT_A8_UNORM                    = 65,
    DXGI_FORMAT_R1_UNORM                    = 66,
    DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
    DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
    DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
    DXGI_FORMAT_BC1_TYPELESS                = 70,
    DXGI_FORMAT_BC1_UNORM                   = 71,
    DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
    DXGI_FORMAT_BC2_TYPELESS                = 73,
    DXGI_FORMAT_BC2_UNORM                   = 74,
    DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
    DXGI_FORMAT_BC3_TYPELESS                = 76,
    DXGI_FORMAT_BC3_UNORM                   = 77,
    DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
    DXGI_FORMAT_BC4_TYPELESS                = 79,
    DXGI_FORMAT_BC4_UNORM                   = 80,
    DXGI_FORMAT_BC4_SNORM                   = 81,
    DXGI_FORMAT_BC5_TYPELESS                = 82,
    DXGI_FORMAT_BC5_UNORM                   = 83,
    DXGI_FORMAT_BC5_SNORM                   = 84,
    DXGI_FORMAT_B5G6R5_UNORM                = 85,
    DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
    DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
    DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
    DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
    DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
    DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
    DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
    DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
    DXGI_FORMAT_BC6H_TYPELESS               = 94,
    DXGI_FORMAT_BC6H_UF16                   = 95,
    DXGI_FORMAT_BC6H_SF16                   = 96,
    DXGI_FORMAT_BC7_TYPELESS                = 97,
    DXGI_FORMAT_BC7_UNORM                   = 98,
    DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
    DXGI_FORMAT_AYUV                        = 100,
    DXGI_FORMAT_Y410                        = 101,
    DXGI_FORMAT_Y416                        = 102,
    DXGI_FORMAT_NV12                        = 103,
    DXGI_FORMAT_P010                        = 104,
    DXGI_FORMAT_P016                        = 105,
    DXGI_FORMAT_420_OPAQUE                  = 106,
    DXGI_FORMAT_YUY2                        = 107,
    DXGI_FORMAT_Y210                        = 108,
    DXGI_FORMAT_Y216                        = 109,
    DXGI_FORMAT_NV11                        = 110,
    DXGI_FORMAT_AI44                        = 111,
    DXGI_FORMAT_IA44                        = 112,
    DXGI_FORMAT_P8                          = 113,
    DXGI_FORMAT_A8P8                        = 114,
    DXGI_FORMAT_B4G4R4A4_UNORM              = 115,

    DXGI_FORMAT_P208                        = 130,
    DXGI_FORMAT_V208                        = 131,
    DXGI_FORMAT_V408                        = 132,


    DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
};

struct DXGI_SAMPLE_DESC
{
    u32 Count;
    u32 Quality;
};


typedef u64 D3D12_GPU_VIRTUAL_ADDRESS;

enum D3D12_HEAP_TYPE
{
    D3D12_HEAP_TYPE_DEFAULT	 = 1,
    D3D12_HEAP_TYPE_UPLOAD   = 2,
    D3D12_HEAP_TYPE_READBACK = 3,
    D3D12_HEAP_TYPE_CUSTOM   = 4
};

enum D3D12_CPU_PAGE_PROPERTY
{
    D3D12_CPU_PAGE_PROPERTY_UNKNOWN	      = 0,
    D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE = 1,
    D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE = 2,
    D3D12_CPU_PAGE_PROPERTY_WRITE_BACK    = 3
};

enum D3D12_MEMORY_POOL
{
    D3D12_MEMORY_POOL_UNKNOWN = 0,
    D3D12_MEMORY_POOL_L0      = 1,
    D3D12_MEMORY_POOL_L1      = 2
};

struct D3D12_HEAP_PROPERTIES
{
    D3D12_HEAP_TYPE         Type;
    D3D12_CPU_PAGE_PROPERTY CPUPageProperty;
    D3D12_MEMORY_POOL       MemoryPoolPreference;
    u32                     CreationNodeMask;
    u32                     VisibleNodeMask;
};

enum D3D12_HEAP_FLAGS
{
    D3D12_HEAP_FLAG_NONE                           = 0,
    D3D12_HEAP_FLAG_SHARED                         = 0x1,
    D3D12_HEAP_FLAG_DENY_BUFFERS                   = 0x4,
    D3D12_HEAP_FLAG_ALLOW_DISPLAY                  = 0x8,
    D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER           = 0x20,
    D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES	           = 0x40,
    D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES	       = 0x80,
    D3D12_HEAP_FLAG_HARDWARE_PROTECTED             = 0x100,
    D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES = 0,
    D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS             = 0xc0,
    D3D12_HEAP_FLAG_ALLOW_ONLY_NON_RT_DS_TEXTURES  = 0x44,
    D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES      = 0x84
};

struct D3D12_HEAP_DESC
{
    u64                   SizeInBytes;
    D3D12_HEAP_PROPERTIES Properties;
    u64                   Alignment;
    D3D12_HEAP_FLAGS      Flags;
};

struct D3D12_RANGE
{
    u64 Begin;
    u64 End;
};

enum D3D12_RESOURCE_DIMENSION
{
    D3D12_RESOURCE_DIMENSION_UNKNOWN   = 0,
    D3D12_RESOURCE_DIMENSION_BUFFER	   = 1,
    D3D12_RESOURCE_DIMENSION_TEXTURE1D = 2,
    D3D12_RESOURCE_DIMENSION_TEXTURE2D = 3,
    D3D12_RESOURCE_DIMENSION_TEXTURE3D = 4
};

enum D3D12_TEXTURE_LAYOUT
{
    D3D12_TEXTURE_LAYOUT_UNKNOWN                = 0,
    D3D12_TEXTURE_LAYOUT_ROW_MAJOR              = 1,
    D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE	= 2,
    D3D12_TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE  = 3
};

enum D3D12_RESOURCE_FLAGS
{
    D3D12_RESOURCE_FLAG_NONE                      = 0,
    D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET	      = 0x1,
    D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL	      = 0x2,
    D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS    = 0x4,
    D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE      = 0x8,
    D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER	      = 0x10,
    D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS = 0x20
};

struct D3D12_RESOURCE_DESC
{
    D3D12_RESOURCE_DIMENSION Dimension;
    u64                      Alignment;
    u64                      Width;
    u32                      Height;
    u16                      DepthOrArraySize;
    u16                      MipLevels;
    DXGI_FORMAT              Format;
    DXGI_SAMPLE_DESC         SampleDesc;
    D3D12_TEXTURE_LAYOUT     Layout;
    D3D12_RESOURCE_FLAGS     Flags;
};

struct D3D12_BOX
{
    u32 left;
    u32 top;
    u32 front;
    u32 right;
    u32 bottom;
    u32 back;
};

enum D3D12_DESCRIPTOR_HEAP_TYPE
{
    D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV = 0,
    D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER     = (D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV + 1),
    D3D12_DESCRIPTOR_HEAP_TYPE_RTV         = (D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER + 1),
    D3D12_DESCRIPTOR_HEAP_TYPE_DSV         = (D3D12_DESCRIPTOR_HEAP_TYPE_RTV + 1),
    D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES   = (D3D12_DESCRIPTOR_HEAP_TYPE_DSV + 1)
};

enum D3D12_DESCRIPTOR_HEAP_FLAGS
{
    D3D12_DESCRIPTOR_HEAP_FLAG_NONE	          = 0,
    D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE = 0x1
};

struct D3D12_DESCRIPTOR_HEAP_DESC
{
    D3D12_DESCRIPTOR_HEAP_TYPE  Type;
    u32                         NumDescriptors;
    D3D12_DESCRIPTOR_HEAP_FLAGS Flags;
    u32                         NodeMask;
};

struct D3D12_CPU_DESCRIPTOR_HANDLE
{
    u64 ptr;
};

struct D3D12_GPU_DESCRIPTOR_HANDLE
{
    u64 ptr;
};

typedef RECT D3D12_RECT;

struct D3D12_DISCARD_REGION
{
    u32              NumRects;
    const D3D12_RECT *pRects;
    u32              FirstSubresource;
    u32              NumSubresources;
};

enum D3D12_COMMAND_LIST_TYPE
{
    D3D12_COMMAND_LIST_TYPE_DIRECT  = 0,
    D3D12_COMMAND_LIST_TYPE_BUNDLE  = 1,
    D3D12_COMMAND_LIST_TYPE_COMPUTE	= 2,
    D3D12_COMMAND_LIST_TYPE_COPY    = 3
};

struct D3D12_SUBRESOURCE_FOOTPRINT
{
    DXGI_FORMAT Format;
    u32         Width;
    u32         Height;
    u32         Depth;
    u32         RowPitch;
};

struct D3D12_PLACED_SUBRESOURCE_FOOTPRINT
{
    u64                         Offset;
    D3D12_SUBRESOURCE_FOOTPRINT Footprint;
};

enum D3D12_TEXTURE_COPY_TYPE
{
    D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX = 0,
    D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT  = 1
};

struct D3D12_TEXTURE_COPY_LOCATION
{
    ID3D12Resource                         *pResource;
    D3D12_TEXTURE_COPY_TYPE                Type;
    union 
    {
        D3D12_PLACED_SUBRESOURCE_FOOTPRINT PlacedFootprint;
        u32                                SubresourceIndex;
    };
};

struct D3D12_TILED_RESOURCE_COORDINATE
{
    u32 X;
    u32 Y;
    u32 Z;
    u32 Subresource;
};

struct D3D12_TILE_REGION_SIZE
{
    u32 NumTiles;
    b32 UseBox;
    u32 Width;
    u16 Height;
    u16 Depth;
};

enum D3D12_TILE_RANGE_FLAGS
{
    D3D12_TILE_RANGE_FLAG_NONE              = 0,
    D3D12_TILE_RANGE_FLAG_NULL              = 1,
    D3D12_TILE_RANGE_FLAG_SKIP              = 2,
    D3D12_TILE_RANGE_FLAG_REUSE_SINGLE_TILE	= 4
};

struct D3D12_SUBRESOURCE_TILING
{
    u32 WidthInTiles;
    u16 HeightInTiles;
    u16 DepthInTiles;
    u32 StartTileIndexInOverallResource;
};

struct D3D12_TILE_SHAPE
{
    u32 WidthInTexels;
    u32 HeightInTexels;
    u32 DepthInTexels;
};

enum D3D12_TILE_MAPPING_FLAGS
{
    D3D12_TILE_MAPPING_FLAG_NONE      = 0,
    D3D12_TILE_MAPPING_FLAG_NO_HAZARD = 0x1
};

enum D3D12_TILE_COPY_FLAGS
{
    D3D12_TILE_COPY_FLAG_NONE                                     = 0,
    D3D12_TILE_COPY_FLAG_NO_HAZARD                                = 0x1,
    D3D12_TILE_COPY_FLAG_LINEAR_BUFFER_TO_SWIZZLED_TILED_RESOURCE = 0x2,
    D3D12_TILE_COPY_FLAG_SWIZZLED_TILED_RESOURCE_TO_LINEAR_BUFFER = 0x4
};

enum D3D12_RESOURCE_STATES
{
    D3D12_RESOURCE_STATE_COMMON	                    = 0,
    D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER	= 0x1,
    D3D12_RESOURCE_STATE_INDEX_BUFFER               = 0x2,
    D3D12_RESOURCE_STATE_RENDER_TARGET              = 0x4,
    D3D12_RESOURCE_STATE_UNORDERED_ACCESS           = 0x8,
    D3D12_RESOURCE_STATE_DEPTH_WRITE                = 0x10,
    D3D12_RESOURCE_STATE_DEPTH_READ                 = 0x20,
    D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE  = 0x40,
    D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE      = 0x80,
    D3D12_RESOURCE_STATE_STREAM_OUT	                = 0x100,
    D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT          = 0x200,
    D3D12_RESOURCE_STATE_COPY_DEST                  = 0x400,
    D3D12_RESOURCE_STATE_COPY_SOURCE                = 0x800,
    D3D12_RESOURCE_STATE_RESOLVE_DEST               = 0x1000,
    D3D12_RESOURCE_STATE_RESOLVE_SOURCE             = 0x2000,
    D3D12_RESOURCE_STATE_GENERIC_READ               = (((((0x1 | 0x2)  | 0x40)  | 0x80)  | 0x200)  | 0x800),
    D3D12_RESOURCE_STATE_PRESENT                    = 0,
    D3D12_RESOURCE_STATE_PREDICATION                = 0x200
};

enum D3D12_RESOURCE_BARRIER_TYPE
{
    D3D12_RESOURCE_BARRIER_TYPE_TRANSITION  = 0,
    D3D12_RESOURCE_BARRIER_TYPE_ALIASING    = (D3D12_RESOURCE_BARRIER_TYPE_TRANSITION + 1),
    D3D12_RESOURCE_BARRIER_TYPE_UAV         = (D3D12_RESOURCE_BARRIER_TYPE_ALIASING + 1) 
};

struct D3D12_RESOURCE_TRANSITION_BARRIER
{
    ID3D12Resource        *pResource;
    u32                   Subresource;
    D3D12_RESOURCE_STATES StateBefore;
    D3D12_RESOURCE_STATES StateAfter;
};

struct D3D12_RESOURCE_ALIASING_BARRIER
{
    ID3D12Resource *pResourceBefore;
    ID3D12Resource *pResourceAfter;
};

struct D3D12_RESOURCE_UAV_BARRIER
{
    ID3D12Resource *pResource;
};

enum D3D12_RESOURCE_BARRIER_FLAGS
{
    D3D12_RESOURCE_BARRIER_FLAG_NONE       = 0,
    D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY = 0x1,
    D3D12_RESOURCE_BARRIER_FLAG_END_ONLY   = 0x2
};

struct D3D12_RESOURCE_BARRIER
{
    D3D12_RESOURCE_BARRIER_TYPE           Type;
    D3D12_RESOURCE_BARRIER_FLAGS          Flags;
    union 
    {
        D3D12_RESOURCE_TRANSITION_BARRIER Transition;
        D3D12_RESOURCE_ALIASING_BARRIER   Aliasing;
        D3D12_RESOURCE_UAV_BARRIER        UAV;
    };
};

enum D3D12_INPUT_CLASSIFICATION
{
    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA   = 0,
    D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA = 1
};

struct D3D12_INPUT_ELEMENT_DESC
{
    const char                 *SemanticName;
    u32                        SemanticIndex;
    DXGI_FORMAT                Format;
    u32                        InputSlot;
    u32                        AlignedByteOffset;
    D3D12_INPUT_CLASSIFICATION InputSlotClass;
    u32                        InstanceDataStepRate;
};

enum D3D12_FILL_MODE
{
    D3D12_FILL_MODE_WIREFRAME = 2,
    D3D12_FILL_MODE_SOLID     = 3
};

enum D3D12_PRIMITIVE_TOPOLOGY
{
    D3D_PRIMITIVE_TOPOLOGY_UNDEFINED                  = 0,
    D3D_PRIMITIVE_TOPOLOGY_POINTLIST                  = 1,
    D3D_PRIMITIVE_TOPOLOGY_LINELIST                   = 2,
    D3D_PRIMITIVE_TOPOLOGY_LINESTRIP                  = 3,
    D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST	              = 4,
    D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP              = 5,
    D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ	              = 10,
    D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ              = 11,
    D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ	          = 12,
    D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ          = 13,
    D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST  = 33,
    D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST  = 34,
    D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST  = 35,
    D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST  = 36,
    D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST  = 37,
    D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST  = 38,
    D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST  = 39,
    D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST  = 40,
    D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST  = 41,
    D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
    D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
    D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
    D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
    D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
    D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
    D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
    D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
    D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
    D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
    D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
    D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
    D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
    D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
    D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
    D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
    D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
    D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
    D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
    D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
    D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
    D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
    D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64
};

enum D3D12_PRIMITIVE_TOPOLOGY_TYPE
{
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED = 0,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT     = 1,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE      = 2,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE  = 3,
    D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH     = 4
};

struct D3D12_SO_DECLARATION_ENTRY
{
    u32        Stream;
    const char *SemanticName;
    u32        SemanticIndex;
    u8         StartComponent;
    u8         ComponentCount;
    u8         OutputSlot;
};

enum D3D12_CULL_MODE
{
    D3D12_CULL_MODE_NONE  = 1,
    D3D12_CULL_MODE_FRONT = 2,
    D3D12_CULL_MODE_BACK  = 3
};

struct D3D12_VIEWPORT
{
    f32 TopLeftX;
    f32 TopLeftY;
    f32 Width;
    f32 Height;
    f32 MinDepth;
    f32 MaxDepth;
};

typedef RECT D3D12_RECT;

enum D3D12_COMPARISON_FUNC
{
    D3D12_COMPARISON_FUNC_NEVER         = 1,
    D3D12_COMPARISON_FUNC_LESS          = 2,
    D3D12_COMPARISON_FUNC_EQUAL         = 3,
    D3D12_COMPARISON_FUNC_LESS_EQUAL    = 4,
    D3D12_COMPARISON_FUNC_GREATER       = 5,
    D3D12_COMPARISON_FUNC_NOT_EQUAL	    = 6,
    D3D12_COMPARISON_FUNC_GREATER_EQUAL = 7,
    D3D12_COMPARISON_FUNC_ALWAYS	    = 8
};

enum D3D12_DEPTH_WRITE_MASK
{
    D3D12_DEPTH_WRITE_MASK_ZERO = 0,
    D3D12_DEPTH_WRITE_MASK_ALL  = 1
};

enum D3D12_STENCIL_OP
{
    D3D12_STENCIL_OP_KEEP     = 1,
    D3D12_STENCIL_OP_ZERO     = 2,
    D3D12_STENCIL_OP_REPLACE  = 3,
    D3D12_STENCIL_OP_INCR_SAT = 4,
    D3D12_STENCIL_OP_DECR_SAT = 5,
    D3D12_STENCIL_OP_INVERT   = 6,
    D3D12_STENCIL_OP_INCR     = 7,
    D3D12_STENCIL_OP_DECR     = 8
};

struct D3D12_DEPTH_STENCILOP_DESC
{
    D3D12_STENCIL_OP      StencilFailOp;
    D3D12_STENCIL_OP      StencilDepthFailOp;
    D3D12_STENCIL_OP      StencilPassOp;
    D3D12_COMPARISON_FUNC StencilFunc;
};

struct D3D12_DEPTH_STENCIL_DESC
{
    b32                        DepthEnable;
    D3D12_DEPTH_WRITE_MASK     DepthWriteMask;
    D3D12_COMPARISON_FUNC      DepthFunc;
    b32                        StencilEnable;
    u8                         StencilReadMask;
    u8                         StencilWriteMask;
    D3D12_DEPTH_STENCILOP_DESC FrontFace;
    D3D12_DEPTH_STENCILOP_DESC BackFace;
};

enum D3D12_BLEND
{
    D3D12_BLEND_ZERO             = 1,
    D3D12_BLEND_ONE              = 2,
    D3D12_BLEND_SRC_COLOR        = 3,
    D3D12_BLEND_INV_SRC_COLOR    = 4,
    D3D12_BLEND_SRC_ALPHA        = 5,
    D3D12_BLEND_INV_SRC_ALPHA    = 6,
    D3D12_BLEND_DEST_ALPHA       = 7,
    D3D12_BLEND_INV_DEST_ALPHA   = 8,
    D3D12_BLEND_DEST_COLOR       = 9,
    D3D12_BLEND_INV_DEST_COLOR   = 10,
    D3D12_BLEND_SRC_ALPHA_SAT    = 11,
    D3D12_BLEND_BLEND_FACTOR     = 14,
    D3D12_BLEND_INV_BLEND_FACTOR = 15,
    D3D12_BLEND_SRC1_COLOR       = 16,
    D3D12_BLEND_INV_SRC1_COLOR   = 17,
    D3D12_BLEND_SRC1_ALPHA       = 18,
    D3D12_BLEND_INV_SRC1_ALPHA   = 19
};

enum D3D12_BLEND_OP
{
    D3D12_BLEND_OP_ADD          = 1,
    D3D12_BLEND_OP_SUBTRACT     = 2,
    D3D12_BLEND_OP_REV_SUBTRACT = 3,
    D3D12_BLEND_OP_MIN          = 4,
    D3D12_BLEND_OP_MAX          = 5
};

enum D3D12_COLOR_WRITE_ENABLE
{
    D3D12_COLOR_WRITE_ENABLE_RED   = 1,
    D3D12_COLOR_WRITE_ENABLE_GREEN = 2,
    D3D12_COLOR_WRITE_ENABLE_BLUE  = 4,
    D3D12_COLOR_WRITE_ENABLE_ALPHA = 8,
    D3D12_COLOR_WRITE_ENABLE_ALL   = (((D3D12_COLOR_WRITE_ENABLE_RED | D3D12_COLOR_WRITE_ENABLE_GREEN) | D3D12_COLOR_WRITE_ENABLE_BLUE) | D3D12_COLOR_WRITE_ENABLE_ALPHA)
};

enum D3D12_LOGIC_OP
{
    D3D12_LOGIC_OP_CLEAR         = 0,
    D3D12_LOGIC_OP_SET           = (D3D12_LOGIC_OP_CLEAR + 1),
    D3D12_LOGIC_OP_COPY          = (D3D12_LOGIC_OP_SET + 1) ,
    D3D12_LOGIC_OP_COPY_INVERTED = (D3D12_LOGIC_OP_COPY + 1),
    D3D12_LOGIC_OP_NOOP          = (D3D12_LOGIC_OP_COPY_INVERTED + 1),
    D3D12_LOGIC_OP_INVERT        = (D3D12_LOGIC_OP_NOOP + 1),
    D3D12_LOGIC_OP_AND           = (D3D12_LOGIC_OP_INVERT + 1),
    D3D12_LOGIC_OP_NAND	         = (D3D12_LOGIC_OP_AND + 1),
    D3D12_LOGIC_OP_OR            = (D3D12_LOGIC_OP_NAND + 1),
    D3D12_LOGIC_OP_NOR           = (D3D12_LOGIC_OP_OR + 1),
    D3D12_LOGIC_OP_XOR           = (D3D12_LOGIC_OP_NOR + 1),
    D3D12_LOGIC_OP_EQUIV         = (D3D12_LOGIC_OP_XOR + 1),
    D3D12_LOGIC_OP_AND_REVERSE   = (D3D12_LOGIC_OP_EQUIV + 1),
    D3D12_LOGIC_OP_AND_INVERTED  = (D3D12_LOGIC_OP_AND_REVERSE + 1),
    D3D12_LOGIC_OP_OR_REVERSE    = (D3D12_LOGIC_OP_AND_INVERTED + 1),
    D3D12_LOGIC_OP_OR_INVERTED   = (D3D12_LOGIC_OP_OR_REVERSE + 1)
};

struct D3D12_RENDER_TARGET_BLEND_DESC
{
    b32            BlendEnable;
    b32            LogicOpEnable;
    D3D12_BLEND    SrcBlend;
    D3D12_BLEND    DestBlend;
    D3D12_BLEND_OP BlendOp;
    D3D12_BLEND    SrcBlendAlpha;
    D3D12_BLEND    DestBlendAlpha;
    D3D12_BLEND_OP BlendOpAlpha;
    D3D12_LOGIC_OP LogicOp;
    u8             RenderTargetWriteMask;
};

struct D3D12_BLEND_DESC
{
    b32                            AlphaToCoverageEnable;
    b32                            IndependentBlendEnable;
    D3D12_RENDER_TARGET_BLEND_DESC RenderTarget[8];
};

enum D3D12_CONSERVATIVE_RASTERIZATION_MODE
{
    D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF = 0,
    D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON  = 1
};

struct D3D12_RASTERIZER_DESC
{
    D3D12_FILL_MODE                       FillMode;
    D3D12_CULL_MODE                       CullMode;
    b32                                   FrontCounterClockwise;
    i32                                   DepthBias;
    f32                                   DepthBiasClamp;
    f32                                   SlopeScaledDepthBias;
    b32                                   DepthClipEnable;
    b32                                   MultisampleEnable;
    b32                                   AntialiasedLineEnable;
    u32                                   ForcedSampleCount;
    D3D12_CONSERVATIVE_RASTERIZATION_MODE ConservativeRaster;
};

enum D3D12_QUERY_HEAP_TYPE
{
    D3D12_QUERY_HEAP_TYPE_OCCLUSION           = 0,
    D3D12_QUERY_HEAP_TYPE_TIMESTAMP           = 1,
    D3D12_QUERY_HEAP_TYPE_PIPELINE_STATISTICS = 2,
    D3D12_QUERY_HEAP_TYPE_SO_STATISTICS       = 3
};

struct D3D12_QUERY_HEAP_DESC
{
    D3D12_QUERY_HEAP_TYPE Type;
    u32                   Count;
    u32                   NodeMask;
};

enum D3D12_QUERY_TYPE
{
    D3D12_QUERY_TYPE_OCCLUSION             = 0,
    D3D12_QUERY_TYPE_BINARY_OCCLUSION      = 1,
    D3D12_QUERY_TYPE_TIMESTAMP             = 2,
    D3D12_QUERY_TYPE_PIPELINE_STATISTICS   = 3,
    D3D12_QUERY_TYPE_SO_STATISTICS_STREAM0 = 4,
    D3D12_QUERY_TYPE_SO_STATISTICS_STREAM1 = 5,
    D3D12_QUERY_TYPE_SO_STATISTICS_STREAM2 = 6,
    D3D12_QUERY_TYPE_SO_STATISTICS_STREAM3 = 7
};

enum D3D12_PREDICATION_OP
{
    D3D12_PREDICATION_OP_EQUAL_ZERO     = 0,
    D3D12_PREDICATION_OP_NOT_EQUAL_ZERO = 1
};

struct D3D12_QUERY_DATA_PIPELINE_STATISTICS
{
    u64 IAVertices;
    u64 IAPrimitives;
    u64 VSInvocations;
    u64 GSInvocations;
    u64 GSPrimitives;
    u64 CInvocations;
    u64 CPrimitives;
    u64 PSInvocations;
    u64 HSInvocations;
    u64 DSInvocations;
    u64 CSInvocations;
};

struct D3D12_QUERY_DATA_SO_STATISTICS
{
    u64 NumPrimitivesWritten;
    u64 PrimitivesStorageNeeded;
};

struct D3D12_STREAM_OUTPUT_BUFFER_VIEW
{
    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation;
    u64                       SizeInBytes;
    D3D12_GPU_VIRTUAL_ADDRESS BufferFilledSizeLocation;
};

struct D3D12_DRAW_ARGUMENTS
{
    u32 VertexCountPerInstance;
    u32 InstanceCount;
    u32 StartVertexLocation;
    u32 StartInstanceLocation;
};

struct D3D12_DRAW_INDEXED_ARGUMENTS
{
    u32 IndexCountPerInstance;
    u32 InstanceCount;
    u32 StartIndexLocation;
    i32 BaseVertexLocation;
    u32 StartInstanceLocation;
};

struct D3D12_DISPATCH_ARGUMENTS
{
    u32 ThreadGroupCountX;
    u32 ThreadGroupCountY;
    u32 ThreadGroupCountZ;
};

struct D3D12_VERTEX_BUFFER_VIEW
{
    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation;
    u32                       SizeInBytes;
    u32                       StrideInBytes;
};

struct D3D12_INDEX_BUFFER_VIEW
{
    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation;
    u32                       SizeInBytes;
    DXGI_FORMAT               Format;
};

enum D3D12_INDIRECT_ARGUMENT_TYPE
{
    D3D12_INDIRECT_ARGUMENT_TYPE_DRAW                  = 0,
    D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED          = (D3D12_INDIRECT_ARGUMENT_TYPE_DRAW + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH              = (D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW    = (D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW     = (D3D12_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT              = (D3D12_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW  = (D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW  = (D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW + 1),
    D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW = (D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW + 1) 
};

struct D3D12_INDIRECT_ARGUMENT_DESC
{
    D3D12_INDIRECT_ARGUMENT_TYPE Type;
    union 
    {
        struct 
        {
                                 u32 Slot;
        } VertexBuffer;
        struct 
        {
                                 u32 RootParameterIndex;
                                 u32 DestOffsetIn32BitValues;
                                 u32 Num32BitValuesToSet;
        } Constant;
        struct 
        {
                                 u32 RootParameterIndex;
        } ConstantBufferView;
        struct 
        {
                                 u32 RootParameterIndex;
        } ShaderResourceView;
        struct 
        {
                                 u32 RootParameterIndex;
        } UnorderedAccessView;
    };
};

struct D3D12_COMMAND_SIGNATURE_DESC
{
    u32                                ByteStride;
    u32                                NumArgumentDescs;
    const D3D12_INDIRECT_ARGUMENT_DESC *pArgumentDescs;
    u32                                NodeMask;
};

enum D3D12_CLEAR_FLAGS
{
    D3D12_CLEAR_FLAG_DEPTH   = 0x1,
    D3D12_CLEAR_FLAG_STENCIL = 0x2
};

enum D3D12_COMMAND_QUEUE_FLAGS
{
    D3D12_COMMAND_QUEUE_FLAG_NONE                = 0,
    D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT = 0x1
};

enum D3D12_COMMAND_QUEUE_PRIORITY
{
    D3D12_COMMAND_QUEUE_PRIORITY_NORMAL = 0,
    D3D12_COMMAND_QUEUE_PRIORITY_HIGH   = 100
};

struct D3D12_COMMAND_QUEUE_DESC
{
    D3D12_COMMAND_LIST_TYPE   Type;
    i32                       Priority;
    D3D12_COMMAND_QUEUE_FLAGS Flags;
    u32                       NodeMask;
};

struct D3D12_SHADER_BYTECODE
{
    const void *pShaderBytecode;
    u64        BytecodeLength;
};

struct D3D12_STREAM_OUTPUT_DESC
{
    const D3D12_SO_DECLARATION_ENTRY *pSODeclaration;
    u32                              NumEntries;
    const u32                        *pBufferStrides;
    u32                              NumStrides;
    u32                              RasterizedStream;
};

struct D3D12_INPUT_LAYOUT_DESC
{
    const D3D12_INPUT_ELEMENT_DESC *pInputElementDescs;
    u32                            NumElements;
};

enum D3D12_INDEX_BUFFER_STRIP_CUT_VALUE
{
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED   = 0,
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF     = 1,
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF = 2
};

struct D3D12_CACHED_PIPELINE_STATE
{
    const void *pCachedBlob;
    u64        CachedBlobSizeInBytes;
};

enum D3D12_PIPELINE_STATE_FLAGS
{
    D3D12_PIPELINE_STATE_FLAG_NONE       = 0,
    D3D12_PIPELINE_STATE_FLAG_TOOL_DEBUG = 0x1
};

struct D3D12_GRAPHICS_PIPELINE_STATE_DESC
{
    ID3D12RootSignature                *pRootSignature;
    D3D12_SHADER_BYTECODE              VS;
    D3D12_SHADER_BYTECODE              PS;
    D3D12_SHADER_BYTECODE              DS;
    D3D12_SHADER_BYTECODE              HS;
    D3D12_SHADER_BYTECODE              GS;
    D3D12_STREAM_OUTPUT_DESC           StreamOutput;
    D3D12_BLEND_DESC                   BlendState;
    u32                                SampleMask;
    D3D12_RASTERIZER_DESC              RasterizerState;
    D3D12_DEPTH_STENCIL_DESC           DepthStencilState;
    D3D12_INPUT_LAYOUT_DESC            InputLayout;
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE IBStripCutValue;
    D3D12_PRIMITIVE_TOPOLOGY_TYPE      PrimitiveTopologyType;
    u32                                NumRenderTargets;
    DXGI_FORMAT                        RTVFormats[8];
    DXGI_FORMAT                        DSVFormat;
    DXGI_SAMPLE_DESC                   SampleDesc;
    u32                                NodeMask;
    D3D12_CACHED_PIPELINE_STATE        CachedPSO;
    D3D12_PIPELINE_STATE_FLAGS         Flags;
};

struct D3D12_COMPUTE_PIPELINE_STATE_DESC
{
    ID3D12RootSignature         *pRootSignature;
    D3D12_SHADER_BYTECODE       CS;
    u32                         NodeMask;
    D3D12_CACHED_PIPELINE_STATE CachedPSO;
    D3D12_PIPELINE_STATE_FLAGS  Flags;
};

enum D3D12_FEATURE
{
    D3D12_FEATURE_D3D12_OPTIONS               = 0,
    D3D12_FEATURE_ARCHITECTURE                = 1,
    D3D12_FEATURE_FEATURE_LEVELS              = 2,
    D3D12_FEATURE_FORMAT_SUPPORT              = 3,
    D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS  = 4,
    D3D12_FEATURE_FORMAT_INFO                 = 5,
    D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT = 6,
    D3D12_FEATURE_SHADER_MODEL                = 7,
    D3D12_FEATURE_D3D12_OPTIONS1              = 8,
    D3D12_FEATURE_ROOT_SIGNATURE              = 12
};

enum D3D12_SHADER_MIN_PRECISION_SUPPORT
{
    D3D12_SHADER_MIN_PRECISION_SUPPORT_NONE   = 0,
    D3D12_SHADER_MIN_PRECISION_SUPPORT_10_BIT = 0x1,
    D3D12_SHADER_MIN_PRECISION_SUPPORT_16_BIT = 0x2
};

enum D3D12_TILED_RESOURCES_TIER
{
    D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED = 0,
    D3D12_TILED_RESOURCES_TIER_1             = 1,
    D3D12_TILED_RESOURCES_TIER_2             = 2,
    D3D12_TILED_RESOURCES_TIER_3             = 3
};

enum D3D12_RESOURCE_BINDING_TIER
{
    D3D12_RESOURCE_BINDING_TIER_1 = 1,
    D3D12_RESOURCE_BINDING_TIER_2 = 2,
    D3D12_RESOURCE_BINDING_TIER_3 = 3
};

enum D3D12_CONSERVATIVE_RASTERIZATION_TIER
{
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED	= 0,
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_1             = 1,
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_2             = 2,
    D3D12_CONSERVATIVE_RASTERIZATION_TIER_3             = 3
};

enum D3D12_FORMAT_SUPPORT1
{
    D3D12_FORMAT_SUPPORT1_NONE                        = 0,
    D3D12_FORMAT_SUPPORT1_BUFFER                      = 0x1,
    D3D12_FORMAT_SUPPORT1_IA_VERTEX_BUFFER            = 0x2,
    D3D12_FORMAT_SUPPORT1_IA_INDEX_BUFFER             = 0x4,
    D3D12_FORMAT_SUPPORT1_SO_BUFFER	                  = 0x8,
    D3D12_FORMAT_SUPPORT1_TEXTURE1D	                  = 0x10,
    D3D12_FORMAT_SUPPORT1_TEXTURE2D	                  = 0x20,
    D3D12_FORMAT_SUPPORT1_TEXTURE3D	                  = 0x40,
    D3D12_FORMAT_SUPPORT1_TEXTURECUBE                 = 0x80,
    D3D12_FORMAT_SUPPORT1_SHADER_LOAD                 = 0x100,
    D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE               = 0x200,
    D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_COMPARISON    = 0x400,
    D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE_MONO_TEXT     = 0x800,
    D3D12_FORMAT_SUPPORT1_MIP                         = 0x1000,
    D3D12_FORMAT_SUPPORT1_RENDER_TARGET               = 0x4000,
    D3D12_FORMAT_SUPPORT1_BLENDABLE                   = 0x8000,
    D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL               = 0x10000,
    D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RESOLVE         = 0x40000,
    D3D12_FORMAT_SUPPORT1_DISPLAY                     = 0x80000,
    D3D12_FORMAT_SUPPORT1_CAST_WITHIN_BIT_LAYOUT      = 0x100000,
    D3D12_FORMAT_SUPPORT1_MULTISAMPLE_RENDERTARGET    = 0x200000,
    D3D12_FORMAT_SUPPORT1_MULTISAMPLE_LOAD            = 0x400000,
    D3D12_FORMAT_SUPPORT1_SHADER_GATHER               = 0x800000,
    D3D12_FORMAT_SUPPORT1_BACK_BUFFER_CAST            = 0x1000000,
    D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW = 0x2000000,
    D3D12_FORMAT_SUPPORT1_SHADER_GATHER_COMPARISON	  = 0x4000000,
    D3D12_FORMAT_SUPPORT1_DECODER_OUTPUT              = 0x8000000,
    D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_OUTPUT      = 0x10000000,
    D3D12_FORMAT_SUPPORT1_VIDEO_PROCESSOR_INPUT       = 0x20000000,
    D3D12_FORMAT_SUPPORT1_VIDEO_ENCODER	              = 0x40000000
};

enum D3D12_FORMAT_SUPPORT2
{
    D3D12_FORMAT_SUPPORT2_NONE                                         = 0,
    D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_ADD                               = 0x1,
    D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_BITWISE_OPS                       = 0x2,
    D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_COMPARE_STORE_OR_COMPARE_EXCHANGE = 0x4,
    D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_EXCHANGE                          = 0x8,
    D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_SIGNED_MIN_OR_MAX                 = 0x10,
    D3D12_FORMAT_SUPPORT2_UAV_ATOMIC_UNSIGNED_MIN_OR_MAX               = 0x20,
    D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD                               = 0x40,
    D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE                              = 0x80,
    D3D12_FORMAT_SUPPORT2_OUTPUT_MERGER_LOGIC_OP                       = 0x100,
    D3D12_FORMAT_SUPPORT2_TILED                                        = 0x200,
    D3D12_FORMAT_SUPPORT2_MULTIPLANE_OVERLAY                           = 0x4000
};

enum D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS
{
    D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE           = 0,
    D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_TILED_RESOURCE = 0x1
};

enum D3D12_CROSS_NODE_SHARING_TIER
{
    D3D12_CROSS_NODE_SHARING_TIER_NOT_SUPPORTED = 0,
    D3D12_CROSS_NODE_SHARING_TIER_1_EMULATED    = 1,
    D3D12_CROSS_NODE_SHARING_TIER_1	            = 2,
    D3D12_CROSS_NODE_SHARING_TIER_2	            = 3
};

enum D3D12_RESOURCE_HEAP_TIER
{
    D3D12_RESOURCE_HEAP_TIER_1 = 1,
    D3D12_RESOURCE_HEAP_TIER_2 = 2
};

struct D3D12_FEATURE_DATA_D3D12_OPTIONS
{
    b32                                   DoublePrecisionFloatShaderOps;
    b32                                   OutputMergerLogicOp;
    D3D12_SHADER_MIN_PRECISION_SUPPORT    MinPrecisionSupport;
    D3D12_TILED_RESOURCES_TIER            TiledResourcesTier;
    D3D12_RESOURCE_BINDING_TIER           ResourceBindingTier;
    b32                                   PSSpecifiedStencilRefSupported;
    b32                                   TypedUAVLoadAdditionalFormats;
    b32                                   ROVsSupported;
    D3D12_CONSERVATIVE_RASTERIZATION_TIER ConservativeRasterizationTier;
    u32                                   MaxGPUVirtualAddressBitsPerResource;
    b32                                   StandardSwizzle64KBSupported;
    D3D12_CROSS_NODE_SHARING_TIER         CrossNodeSharingTier;
    b32                                   CrossAdapterRowMajorTextureSupported;
    b32                                   VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation;
    D3D12_RESOURCE_HEAP_TIER              ResourceHeapTier;
};

struct D3D12_FEATURE_DATA_D3D12_OPTIONS1
{
    b32 WaveOps;
    u32 WaveLaneCountMin;
    u32 WaveLaneCountMax;
    u32 TotalLaneCount;
    b32 ExpandedComputeResourceStates;
    b32 Int64ShaderOps;
};

enum D3D_ROOT_SIGNATURE_VERSION
{
    D3D_ROOT_SIGNATURE_VERSION_1   = 0x1,
    D3D_ROOT_SIGNATURE_VERSION_1_0 = 0x1,
    D3D_ROOT_SIGNATURE_VERSION_1_1 = 0x2
};

struct D3D12_FEATURE_DATA_ROOT_SIGNATURE
{
    D3D_ROOT_SIGNATURE_VERSION HighestVersion;
};

struct D3D12_FEATURE_DATA_ARCHITECTURE
{
    u32 NodeIndex;
    b32 TileBasedRenderer;
    b32 UMA;
    b32 CacheCoherentUMA;
};

enum D3D_FEATURE_LEVEL
{
    D3D_FEATURE_LEVEL_9_1  = 0x9100,
    D3D_FEATURE_LEVEL_9_2  = 0x9200,
    D3D_FEATURE_LEVEL_9_3  = 0x9300,
    D3D_FEATURE_LEVEL_10_0 = 0xa000,
    D3D_FEATURE_LEVEL_10_1 = 0xa100,
    D3D_FEATURE_LEVEL_11_0 = 0xb000,
    D3D_FEATURE_LEVEL_11_1 = 0xb100,
    D3D_FEATURE_LEVEL_12_0 = 0xc000,
    D3D_FEATURE_LEVEL_12_1 = 0xc100
};

struct D3D12_FEATURE_DATA_FEATURE_LEVELS
{
    u32                     NumFeatureLevels;
    const D3D_FEATURE_LEVEL *pFeatureLevelsRequested;
    D3D_FEATURE_LEVEL       MaxSupportedFeatureLevel;
};

enum D3D_SHADER_MODEL
{
    D3D_SHADER_MODEL_5_1 = 0x51,
    D3D_SHADER_MODEL_6_0 = 0x60
};

struct D3D12_FEATURE_DATA_SHADER_MODEL
{
    D3D_SHADER_MODEL HighestShaderModel;
};

struct D3D12_FEATURE_DATA_FORMAT_SUPPORT
{
    DXGI_FORMAT           Format;
    D3D12_FORMAT_SUPPORT1 Support1;
    D3D12_FORMAT_SUPPORT2 Support2;
};

struct D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS
{
    DXGI_FORMAT                           Format;
    u32                                   SampleCount;
    D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS Flags;
    u32                                   NumQualityLevels;
};

struct D3D12_FEATURE_DATA_FORMAT_INFO
{
    DXGI_FORMAT Format;
    u8          PlaneCount;
};

struct D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT
{
    u32 MaxGPUVirtualAddressBitsPerResource;
    u32 MaxGPUVirtualAddressBitsPerProcess;
};

struct D3D12_RESOURCE_ALLOCATION_INFO
{
    u64 SizeInBytes;
    u64 Alignment;
};

enum D3D12_SHADER_COMPONENT_MAPPING
{
    D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0 = 0,
    D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1 = 1,
    D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2 = 2,
    D3D12_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3 = 3,
    D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_0           = 4,
    D3D12_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1           = 5
};

#define D3D12_SHADER_COMPONENT_MAPPING_MASK 0x7 
#define D3D12_SHADER_COMPONENT_MAPPING_SHIFT 3 
#define D3D12_SHADER_COMPONENT_MAPPING_ALWAYS_SET_BIT_AVOIDING_ZEROMEM_MISTAKES (1<<(D3D12_SHADER_COMPONENT_MAPPING_SHIFT*4)) 
#define D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(Src0,Src1,Src2,Src3) ((((Src0)&D3D12_SHADER_COMPONENT_MAPPING_MASK)| \
                                                                (((Src1)&D3D12_SHADER_COMPONENT_MAPPING_MASK)<<D3D12_SHADER_COMPONENT_MAPPING_SHIFT)| \
                                                                (((Src2)&D3D12_SHADER_COMPONENT_MAPPING_MASK)<<(D3D12_SHADER_COMPONENT_MAPPING_SHIFT*2))| \
                                                                (((Src3)&D3D12_SHADER_COMPONENT_MAPPING_MASK)<<(D3D12_SHADER_COMPONENT_MAPPING_SHIFT*3))| \
                                                                D3D12_SHADER_COMPONENT_MAPPING_ALWAYS_SET_BIT_AVOIDING_ZEROMEM_MISTAKES))
#define D3D12_DECODE_SHADER_4_COMPONENT_MAPPING(ComponentToExtract,Mapping) ((D3D12_SHADER_COMPONENT_MAPPING)(Mapping >> (D3D12_SHADER_COMPONENT_MAPPING_SHIFT*ComponentToExtract) & D3D12_SHADER_COMPONENT_MAPPING_MASK))
#define D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING D3D12_ENCODE_SHADER_4_COMPONENT_MAPPING(0,1,2,3) 

enum D3D12_BUFFER_SRV_FLAGS
{
    D3D12_BUFFER_SRV_FLAG_NONE = 0,
    D3D12_BUFFER_SRV_FLAG_RAW  = 0x1
};

struct D3D12_BUFFER_SRV
{
    u64                    FirstElement;
    u32                    NumElements;
    u32                    StructureByteStride;
    D3D12_BUFFER_SRV_FLAGS Flags;
};

struct D3D12_TEX1D_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEX1D_ARRAY_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    u32 FirstArraySlice;
    u32 ArraySize;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEX2D_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    u32 PlaneSlice;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEX2D_ARRAY_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    u32 FirstArraySlice;
    u32 ArraySize;
    u32 PlaneSlice;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEX3D_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEXCUBE_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEXCUBE_ARRAY_SRV
{
    u32 MostDetailedMip;
    u32 MipLevels;
    u32 First2DArrayFace;
    u32 NumCubes;
    f32 ResourceMinLODClamp;
};

struct D3D12_TEX2DMS_SRV
{
    u32 UnusedField_NothingToDefine;
};

struct D3D12_TEX2DMS_ARRAY_SRV
{
    u32 FirstArraySlice;
    u32 ArraySize;
};

enum D3D12_SRV_DIMENSION
{
    D3D12_SRV_DIMENSION_UNKNOWN          = 0,
    D3D12_SRV_DIMENSION_BUFFER           = 1,
    D3D12_SRV_DIMENSION_TEXTURE1D        = 2,
    D3D12_SRV_DIMENSION_TEXTURE1DARRAY   = 3,
    D3D12_SRV_DIMENSION_TEXTURE2D        = 4,
    D3D12_SRV_DIMENSION_TEXTURE2DARRAY   = 5,
    D3D12_SRV_DIMENSION_TEXTURE2DMS      = 6,
    D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY = 7,
    D3D12_SRV_DIMENSION_TEXTURE3D        = 8,
    D3D12_SRV_DIMENSION_TEXTURECUBE      = 9,
    D3D12_SRV_DIMENSION_TEXTURECUBEARRAY = 10
};

struct D3D12_SHADER_RESOURCE_VIEW_DESC
{
    DXGI_FORMAT                 Format;
    D3D12_SRV_DIMENSION         ViewDimension;
    u32                         Shader4ComponentMapping;
    union
    {
        D3D12_BUFFER_SRV        Buffer;
        D3D12_TEX1D_SRV         Texture1D;
        D3D12_TEX1D_ARRAY_SRV   Texture1DArray;
        D3D12_TEX2D_SRV         Texture2D;
        D3D12_TEX2D_ARRAY_SRV   Texture2DArray;
        D3D12_TEX2DMS_SRV       Texture2DMS;
        D3D12_TEX2DMS_ARRAY_SRV Texture2DMSArray;
        D3D12_TEX3D_SRV         Texture3D;
        D3D12_TEXCUBE_SRV       TextureCube;
        D3D12_TEXCUBE_ARRAY_SRV TextureCubeArray;
    };
};

enum D3D12_FILTER
{
    D3D12_FILTER_MIN_MAG_MIP_POINT                          = 0,
    D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR                   = 0x1,
    D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT             = 0x4,
    D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR                   = 0x5,
    D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT                   = 0x10,
    D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR            = 0x11,
    D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT                   = 0x14,
    D3D12_FILTER_MIN_MAG_MIP_LINEAR                         = 0x15,
    D3D12_FILTER_ANISOTROPIC                                = 0x55,
    D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT               = 0x80,
    D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR        = 0x81,
    D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT	= 0x84,
    D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR        = 0x85,
    D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT        = 0x90,
    D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x91,
    D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT        = 0x94,
    D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR              = 0x95,
    D3D12_FILTER_COMPARISON_ANISOTROPIC                     = 0xd5,
    D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT                  = 0x100,
    D3D12_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR           = 0x101,
    D3D12_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT     = 0x104,
    D3D12_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR           = 0x105,
    D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT           = 0x110,
    D3D12_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR    = 0x111,
    D3D12_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT           = 0x114,
    D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR	                = 0x115,
    D3D12_FILTER_MINIMUM_ANISOTROPIC                        = 0x155,
    D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT                  = 0x180,
    D3D12_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR           = 0x181,
    D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT     = 0x184,
    D3D12_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR           = 0x185,
    D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT           = 0x190,
    D3D12_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR	= 0x191,
    D3D12_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT           = 0x194,
    D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR                 = 0x195,
    D3D12_FILTER_MAXIMUM_ANISOTROPIC                        = 0x1d5
};

enum D3D12_FILTER_TYPE
{
    D3D12_FILTER_TYPE_POINT  = 0,
    D3D12_FILTER_TYPE_LINEAR = 1
};

enum D3D12_FILTER_REDUCTION_TYPE
{
    D3D12_FILTER_REDUCTION_TYPE_STANDARD   = 0,
    D3D12_FILTER_REDUCTION_TYPE_COMPARISON = 1,
    D3D12_FILTER_REDUCTION_TYPE_MINIMUM    = 2,
    D3D12_FILTER_REDUCTION_TYPE_MAXIMUM    = 3
};

#define	D3D12_FILTER_REDUCTION_TYPE_MASK 0x3
#define	D3D12_FILTER_REDUCTION_TYPE_SHIFT 7
#define	D3D12_FILTER_TYPE_MASK 0x3
#define	D3D12_MIN_FILTER_SHIFT 4
#define	D3D12_MAG_FILTER_SHIFT 2
#define	D3D12_MIP_FILTER_SHIFT 0
#define	D3D12_ANISOTROPIC_FILTERING_BIT 0x40

#define D3D12_ENCODE_BASIC_FILTER( min, mag, mip, reduction )                                                     \
                                   ( ( D3D12_FILTER ) (                                                           \
                                   ( ( ( min ) & D3D12_FILTER_TYPE_MASK ) << D3D12_MIN_FILTER_SHIFT ) |           \
                                   ( ( ( mag ) & D3D12_FILTER_TYPE_MASK ) << D3D12_MAG_FILTER_SHIFT ) |           \
                                   ( ( ( mip ) & D3D12_FILTER_TYPE_MASK ) << D3D12_MIP_FILTER_SHIFT ) |           \
                                   ( ( ( reduction ) & D3D12_FILTER_REDUCTION_TYPE_MASK ) << D3D12_FILTER_REDUCTION_TYPE_SHIFT ) ) ) 
#define D3D12_ENCODE_ANISOTROPIC_FILTER( reduction )                                                  \
                                         ( ( D3D12_FILTER ) (                                         \
                                         D3D12_ANISOTROPIC_FILTERING_BIT |                            \
                                         D3D12_ENCODE_BASIC_FILTER( D3D12_FILTER_TYPE_LINEAR,         \
                                                                    D3D12_FILTER_TYPE_LINEAR,         \
                                                                    D3D12_FILTER_TYPE_LINEAR,         \
                                                                    reduction ) ) )                     
#define D3D12_DECODE_MIN_FILTER( D3D12Filter )                                                              \
                                 ( ( D3D12_FILTER_TYPE )                                                    \
                                 ( ( ( D3D12Filter ) >> D3D12_MIN_FILTER_SHIFT ) & D3D12_FILTER_TYPE_MASK ) ) 
#define D3D12_DECODE_MAG_FILTER( D3D12Filter )                                                              \
                                 ( ( D3D12_FILTER_TYPE )                                                    \
                                 ( ( ( D3D12Filter ) >> D3D12_MAG_FILTER_SHIFT ) & D3D12_FILTER_TYPE_MASK ) ) 
#define D3D12_DECODE_MIP_FILTER( D3D12Filter )                                                              \
                                 ( ( D3D12_FILTER_TYPE )                                                    \
                                 ( ( ( D3D12Filter ) >> D3D12_MIP_FILTER_SHIFT ) & D3D12_FILTER_TYPE_MASK ) ) 
#define D3D12_DECODE_FILTER_REDUCTION( D3D12Filter )                                                        \
                                 ( ( D3D12_FILTER_REDUCTION_TYPE )                                                      \
                                 ( ( ( D3D12Filter ) >> D3D12_FILTER_REDUCTION_TYPE_SHIFT ) & D3D12_FILTER_REDUCTION_TYPE_MASK ) ) 
#define D3D12_DECODE_IS_COMPARISON_FILTER( D3D12Filter )                                                    \
                                 ( D3D12_DECODE_FILTER_REDUCTION( D3D12Filter ) == D3D12_FILTER_REDUCTION_TYPE_COMPARISON ) 
#define D3D12_DECODE_IS_ANISOTROPIC_FILTER( D3D12Filter )                                               \
                            ( ( ( D3D12Filter ) & D3D12_ANISOTROPIC_FILTERING_BIT ) &&                  \
                            ( D3D12_FILTER_TYPE_LINEAR == D3D12_DECODE_MIN_FILTER( D3D12Filter ) ) &&   \
                            ( D3D12_FILTER_TYPE_LINEAR == D3D12_DECODE_MAG_FILTER( D3D12Filter ) ) &&   \
                            ( D3D12_FILTER_TYPE_LINEAR == D3D12_DECODE_MIP_FILTER( D3D12Filter ) ) )      

enum D3D12_TEXTURE_ADDRESS_MODE
{
    D3D12_TEXTURE_ADDRESS_MODE_WRAP        = 1,
    D3D12_TEXTURE_ADDRESS_MODE_MIRROR      = 2,
    D3D12_TEXTURE_ADDRESS_MODE_CLAMP       = 3,
    D3D12_TEXTURE_ADDRESS_MODE_BORDER      = 4,
    D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE = 5
};

struct D3D12_SAMPLER_DESC
{
    D3D12_FILTER               Filter;
    D3D12_TEXTURE_ADDRESS_MODE AddressU;
    D3D12_TEXTURE_ADDRESS_MODE AddressV;
    D3D12_TEXTURE_ADDRESS_MODE AddressW;
    f32                        MipLODBias;
    u32                        MaxAnisotropy;
    D3D12_COMPARISON_FUNC      ComparisonFunc;
    f32                        BorderColor[4];
    f32                        MinLOD;
    f32                        MaxLOD;
};

struct D3D12_CONSTANT_BUFFER_VIEW_DESC
{
    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation;
    u32                       SizeInBytes;
};

enum D3D12_BUFFER_UAV_FLAGS
{
    D3D12_BUFFER_UAV_FLAG_NONE = 0,
    D3D12_BUFFER_UAV_FLAG_RAW  = 0x1
};

struct D3D12_BUFFER_UAV
{
    u64                    FirstElement;
    u32                    NumElements;
    u32                    StructureByteStride;
    u64                    CounterOffsetInBytes;
    D3D12_BUFFER_UAV_FLAGS Flags;
};

struct D3D12_TEX1D_UAV
{
    u32 MipSlice;
};

struct D3D12_TEX1D_ARRAY_UAV
{
    u32 MipSlice;
    u32 FirstArraySlice;
    u32 ArraySize;
};

struct D3D12_TEX2D_UAV
{
    u32 MipSlice;
    u32 PlaneSlice;
};

struct D3D12_TEX2D_ARRAY_UAV
{
    u32 MipSlice;
    u32 FirstArraySlice;
    u32 ArraySize;
    u32 PlaneSlice;
};

struct D3D12_TEX3D_UAV
{
    u32 MipSlice;
    u32 FirstWSlice;
    u32 WSize;
};

enum D3D12_UAV_DIMENSION
{
    D3D12_UAV_DIMENSION_UNKNOWN        = 0,
    D3D12_UAV_DIMENSION_BUFFER         = 1,
    D3D12_UAV_DIMENSION_TEXTURE1D      = 2,
    D3D12_UAV_DIMENSION_TEXTURE1DARRAY = 3,
    D3D12_UAV_DIMENSION_TEXTURE2D      = 4,
    D3D12_UAV_DIMENSION_TEXTURE2DARRAY = 5,
    D3D12_UAV_DIMENSION_TEXTURE3D      = 8
};

struct D3D12_UNORDERED_ACCESS_VIEW_DESC
{
    DXGI_FORMAT               Format;
    D3D12_UAV_DIMENSION       ViewDimension;
    union 
    {
        D3D12_BUFFER_UAV      Buffer;
        D3D12_TEX1D_UAV       Texture1D;
        D3D12_TEX1D_ARRAY_UAV Texture1DArray;
        D3D12_TEX2D_UAV       Texture2D;
        D3D12_TEX2D_ARRAY_UAV Texture2DArray;
        D3D12_TEX3D_UAV       Texture3D;
    };
};

struct D3D12_BUFFER_RTV
{
    u64 FirstElement;
    u32 NumElements;
};

struct D3D12_TEX1D_RTV
{
    u32 MipSlice;
};

struct D3D12_TEX1D_ARRAY_RTV
{
    u32 MipSlice;
    u32 FirstArraySlice;
    u32 ArraySize;
};

struct D3D12_TEX2D_RTV
{
    u32 MipSlice;
    u32 PlaneSlice;
};

struct D3D12_TEX2DMS_RTV
{
    u32 UnusedField_NothingToDefine;
};

struct D3D12_TEX2D_ARRAY_RTV
{
    u32 MipSlice;
    u32 FirstArraySlice;
    u32 ArraySize;
    u32 PlaneSlice;
};

struct D3D12_TEX2DMS_ARRAY_RTV
{
    u32 FirstArraySlice;
    u32 ArraySize;
};

struct D3D12_TEX3D_RTV
{
    u32 MipSlice;
    u32 FirstWSlice;
    u32 WSize;
};

enum D3D12_RTV_DIMENSION
{
    D3D12_RTV_DIMENSION_UNKNOWN          = 0,
    D3D12_RTV_DIMENSION_BUFFER           = 1,
    D3D12_RTV_DIMENSION_TEXTURE1D        = 2,
    D3D12_RTV_DIMENSION_TEXTURE1DARRAY	 = 3,
    D3D12_RTV_DIMENSION_TEXTURE2D        = 4,
    D3D12_RTV_DIMENSION_TEXTURE2DARRAY   = 5,
    D3D12_RTV_DIMENSION_TEXTURE2DMS      = 6,
    D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY = 7,
    D3D12_RTV_DIMENSION_TEXTURE3D        = 8
};

struct D3D12_RENDER_TARGET_VIEW_DESC
{
    DXGI_FORMAT                 Format;
    D3D12_RTV_DIMENSION         ViewDimension;
    union 
    {
        D3D12_BUFFER_RTV        Buffer;
        D3D12_TEX1D_RTV         Texture1D;
        D3D12_TEX1D_ARRAY_RTV   Texture1DArray;
        D3D12_TEX2D_RTV         Texture2D;
        D3D12_TEX2D_ARRAY_RTV   Texture2DArray;
        D3D12_TEX2DMS_RTV       Texture2DMS;
        D3D12_TEX2DMS_ARRAY_RTV Texture2DMSArray;
        D3D12_TEX3D_RTV         Texture3D;
    };
};

struct D3D12_TEX1D_DSV
{
    u32 MipSlice;
};

struct D3D12_TEX1D_ARRAY_DSV
{
    u32 MipSlice;
    u32 FirstArraySlice;
    u32 ArraySize;
};

struct D3D12_TEX2D_DSV
{
    u32 MipSlice;
};

struct D3D12_TEX2D_ARRAY_DSV
{
    u32 MipSlice;
    u32 FirstArraySlice;
    u32 ArraySize;
};

struct D3D12_TEX2DMS_DSV
{
    u32 UnusedField_NothingToDefine;
};

struct D3D12_TEX2DMS_ARRAY_DSV
{
    u32 FirstArraySlice;
    u32 ArraySize;
};

enum D3D12_DSV_FLAGS
{
    D3D12_DSV_FLAG_NONE	             = 0,
    D3D12_DSV_FLAG_READ_ONLY_DEPTH	 = 0x1,
    D3D12_DSV_FLAG_READ_ONLY_STENCIL = 0x2
};

enum D3D12_DSV_DIMENSION
{
    D3D12_DSV_DIMENSION_UNKNOWN          = 0,
    D3D12_DSV_DIMENSION_TEXTURE1D        = 1,
    D3D12_DSV_DIMENSION_TEXTURE1DARRAY   = 2,
    D3D12_DSV_DIMENSION_TEXTURE2D        = 3,
    D3D12_DSV_DIMENSION_TEXTURE2DARRAY   = 4,
    D3D12_DSV_DIMENSION_TEXTURE2DMS      = 5,
    D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY = 6
};

struct D3D12_DEPTH_STENCIL_VIEW_DESC
{
    DXGI_FORMAT                 Format;
    D3D12_DSV_DIMENSION         ViewDimension;
    D3D12_DSV_FLAGS             Flags;
    union 
    {
        D3D12_TEX1D_DSV         Texture1D;
        D3D12_TEX1D_ARRAY_DSV   Texture1DArray;
        D3D12_TEX2D_DSV         Texture2D;
        D3D12_TEX2D_ARRAY_DSV   Texture2DArray;
        D3D12_TEX2DMS_DSV       Texture2DMS;
        D3D12_TEX2DMS_ARRAY_DSV Texture2DMSArray;
    };
};

struct D3D12_DEPTH_STENCIL_VALUE
{
    f32 Depth;
    u8  Stencil;
};

struct D3D12_CLEAR_VALUE
{
    DXGI_FORMAT Format;
    union 
    {
        f32                       Color[4];
        D3D12_DEPTH_STENCIL_VALUE DepthStencil;
    };
};

struct SECURITY_ATTRIBUTES
{
    u32  nLength;
    void *lpSecurityDescriptor;
    b32  bInheritHandle;
};

enum D3D12_FENCE_FLAGS
{
    D3D12_FENCE_FLAG_NONE                 = 0,
    D3D12_FENCE_FLAG_SHARED               = 0x1,
    D3D12_FENCE_FLAG_SHARED_CROSS_ADAPTER = 0x2
};

struct D3D12_PACKED_MIP_INFO
{
    u8  NumStandardMips;
    u8  NumPackedMips;
    u32 NumTilesForPackedMips;
    u32 StartTileIndexInOverallResource;
};

struct D3D12_SUBRESOURCE_DATA
{
    const void *pData;
    i64        RowPitch;
    i64        SlicePitch;
};

struct D3D12_MEMCPY_DEST
{
    void *pData;
    u64  RowPitch;
    u64  SlicePitch;
};


struct IUnknown
{
    virtual i32 STDCALL QueryInterface(const GUID &riid, void **ppvObject) = 0;
    virtual u32 STDCALL AddRef() = 0;
    virtual u32 STDCALL Release() = 0;
};


struct ID3DBlob : public IUnknown
{
    virtual void * STDCALL GetBufferPointer() = 0;
    virtual u64    STDCALL GetBufferSize() = 0;
};

struct ID3D12Object : public IUnknown
{
    virtual i32 STDCALL GetPrivateData(const GUID &guid, u32 *pDataSize, void *pData) = 0;
    virtual i32 STDCALL SetPrivateData(const GUID &guid, u32 DataSize, const void *pData) = 0;
    virtual i32 STDCALL SetPrivateDataInterface(const GUID &guid, const IUnknown *pData) = 0;
    virtual i32 STDCALL SetName(const wchar_t *Name) = 0;
};

struct ID3D12DeviceChild : public ID3D12Object
{
    virtual i32 STDCALL GetDevice(const GUID &riid, void **ppvDevice) = 0;
};

struct ID3D12RootSignature : public ID3D12DeviceChild
{
};

struct ID3D12Pageable : public ID3D12DeviceChild
{
};

struct ID3D12Heap : public ID3D12Pageable
{
    virtual D3D12_HEAP_DESC STDCALL GetDesc() = 0;
};

struct ID3D12Resource : public ID3D12Pageable
{
    virtual i32                       STDCALL Map(u32 Subresource, const D3D12_RANGE *pReadRange, void **ppData) = 0;
    virtual void                      STDCALL Unmap(u32 Subresource, const D3D12_RANGE *pWrittenRange) = 0;
    virtual D3D12_RESOURCE_DESC       STDCALL GetDesc() = 0;
    virtual D3D12_GPU_VIRTUAL_ADDRESS STDCALL GetGPUVirtualAddress() = 0;
    virtual i32                       STDCALL WriteToSubresource(u32 DstSubresource, const D3D12_BOX *pDstBox,
                                                                 const void *pSrcData, u32 SrcRowPitch,
                                                                 u32 SrcDepthPitch) = 0;
    virtual i32                       STDCALL ReadFromSubresource(void *pDstData, u32 DstRowPitch,
                                                                  u32 DstDepthPitch, u32 SrcSubresource,
                                                                  const D3D12_BOX *pSrcBox) = 0;
    virtual i32                       STDCALL GetHeapProperties(D3D12_HEAP_PROPERTIES *pHeapProperties,
                                                                D3D12_HEAP_FLAGS *pHeapFlags) = 0;
};

struct ID3D12CommandAllocator : public ID3D12Pageable
{
    virtual i32 STDCALL Reset() = 0;
};

struct ID3D12Fence : public ID3D12Pageable
{
    virtual u64 STDCALL GetCompletedValue() = 0;
    virtual i32 STDCALL SetEventOnCompletion(u64 Value, void *hEvent) = 0;
    virtual i32 STDCALL Signal(u64 Value) = 0;
};

struct ID3D12PipelineState : public ID3D12Pageable
{
    virtual i32 STDCALL GetCachedBlob(ID3DBlob **ppBlob) = 0;
};

struct ID3D12DescriptorHeap : public ID3D12Pageable
{
    virtual D3D12_DESCRIPTOR_HEAP_DESC  STDCALL GetDesc() = 0;
    virtual D3D12_CPU_DESCRIPTOR_HANDLE STDCALL GetCPUDescriptorHandleForHeapStart() = 0;
    virtual D3D12_GPU_DESCRIPTOR_HANDLE STDCALL GetGPUDescriptorHandleForHeapStart() = 0;
};

struct ID3D12QueryHeap : public ID3D12Pageable
{
};

struct ID3D12CommandSignature : public ID3D12Pageable
{
};

struct ID3D12CommandList : public ID3D12DeviceChild
{
    virtual D3D12_COMMAND_LIST_TYPE STDCALL GetType() = 0;
};

struct ID3D12GraphicsCommandList : public ID3D12CommandList
{
    virtual i32  STDCALL Close() = 0;
    virtual i32  STDCALL Reset(ID3D12CommandAllocator *pAllocator, ID3D12PipelineState *pInitialState) = 0;
    virtual void STDCALL ClearState(ID3D12PipelineState *pPipelineState) = 0;
    virtual void STDCALL DrawInstanced(u32 VertexCountPerInstance, u32 InstanceCount, u32 StartVertexLocation,
                                       u32 StartInstanceLocation) = 0;
    virtual void STDCALL DrawIndexedInstanced(u32 IndexCountPerInstance, u32 InstanceCount, u32 StartIndexLocation, i32 BaseVertexLocation,
                                              u32 StartInstanceLocation) = 0;
    virtual void STDCALL Dispatch(u32 ThreadGroupCountX, u32 ThreadGroupCountY, u32 ThreadGroupCountZ) = 0;
    virtual void STDCALL CopyBufferRegion(ID3D12Resource *pDstBuffer, u64 DstOffset, ID3D12Resource *pSrcBuffer, u64 SrcOffset,
                                          u64 NumBytes) = 0;
    virtual void STDCALL CopyTextureRegion(const D3D12_TEXTURE_COPY_LOCATION *pDst, u32 DstX, u32 DstY, u32 DstZ,
                                           const D3D12_TEXTURE_COPY_LOCATION *pSrc,
                                           const D3D12_BOX *pSrcBox) = 0;
    virtual void STDCALL CopyResource(ID3D12Resource *pDstResource, ID3D12Resource *pSrcResource) = 0;
    virtual void STDCALL CopyTiles(ID3D12Resource *pTiledResource, const D3D12_TILED_RESOURCE_COORDINATE *pTileRegionStartCoordinate,
                                   const D3D12_TILE_REGION_SIZE *pTileRegionSize, ID3D12Resource *pBuffer,
                                   u64 BufferStartOffsetInBytes, D3D12_TILE_COPY_FLAGS Flags) = 0;
    virtual void STDCALL ResolveSubresource(ID3D12Resource *pDstResource, u32 DstSubresource, ID3D12Resource *pSrcResource,
                                            u32 SrcSubresource, DXGI_FORMAT Format) = 0;
    virtual void STDCALL IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology) = 0;
    virtual void STDCALL RSSetViewports(u32 NumViewports, const D3D12_VIEWPORT *pViewports) = 0;
    virtual void STDCALL RSSetScissorRects(u32 NumRects, const D3D12_RECT *pRects) = 0;
    virtual void STDCALL OMSetBlendFactor(const f32 BlendFactor[4]) = 0;
    virtual void STDCALL OMSetStencilRef(u32 StencilRef) = 0;
    virtual void STDCALL SetPipelineState(ID3D12PipelineState *pPipelineState) = 0;
    virtual void STDCALL ResourceBarrier(u32 NumBarriers, const D3D12_RESOURCE_BARRIER *pBarriers) = 0;
    virtual void STDCALL ExecuteBundle(ID3D12GraphicsCommandList *pCommandList) = 0;
    virtual void STDCALL SetDescriptorHeaps(u32 NumDescriptorHeaps, ID3D12DescriptorHeap *const *ppDescriptorHeaps) = 0;
    virtual void STDCALL SetComputeRootSignature(ID3D12RootSignature *pRootSignature) = 0;
    virtual void STDCALL SetGraphicsRootSignature(ID3D12RootSignature *pRootSignature) = 0; 
    virtual void STDCALL SetComputeRootDescriptorTable(u32 RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) = 0;
    virtual void STDCALL SetGraphicsRootDescriptorTable(u32 RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) = 0;
    virtual void STDCALL SetComputeRoot32BitConstant(u32 RootParameterIndex, u32 SrcData, u32 DestOffsetIn32BitValues) = 0;
    virtual void STDCALL SetGraphicsRoot32BitConstant(u32 RootParameterIndex, u32 SrcData, u32 DestOffsetIn32BitValues) = 0;
    virtual void STDCALL SetComputeRoot32BitConstants(u32 RootParameterIndex, u32 Num32BitValuesToSet, const void *pSrcData,
                                                      u32 DestOffsetIn32BitValues) = 0;
    virtual void STDCALL SetGraphicsRoot32BitConstants(u32 RootParameterIndex, u32 Num32BitValuesToSet, const void *pSrcData,
                                                       u32 DestOffsetIn32BitValues) = 0;
    virtual void STDCALL SetComputeRootConstantBufferView(u32 RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;
    virtual void STDCALL SetGraphicsRootConstantBufferView(u32 RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;
    virtual void STDCALL SetComputeRootShaderResourceView(u32 RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;
    virtual void STDCALL SetGraphicsRootShaderResourceView(u32 RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;
    virtual void STDCALL SetComputeRootUnorderedAccessView(u32 RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;
    virtual void STDCALL SetGraphicsRootUnorderedAccessView(u32 RootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) = 0;
    virtual void STDCALL IASetIndexBuffer(const D3D12_INDEX_BUFFER_VIEW *pView) = 0;
    virtual void STDCALL IASetVertexBuffers(u32 StartSlot, u32 NumViews, const D3D12_VERTEX_BUFFER_VIEW *pViews) = 0;
    virtual void STDCALL SOSetTargets(u32 StartSlot, u32 NumViews, const D3D12_STREAM_OUTPUT_BUFFER_VIEW *pViews) = 0;
    virtual void STDCALL OMSetRenderTargets(u32 NumRenderTargetDescriptors, const D3D12_CPU_DESCRIPTOR_HANDLE *pRenderTargetDescriptors,
                                            b32 RTsSingleHandleToDescriptorRange,
                                            const D3D12_CPU_DESCRIPTOR_HANDLE *pDepthStencilDescriptor) = 0;
    virtual void STDCALL ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView, D3D12_CLEAR_FLAGS ClearFlags, f32 Depth,
                                               u8 Stencil, u32 NumRects, const D3D12_RECT *pRects) = 0;
    virtual void STDCALL ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView, const f32 ColorRGBA[4], u32 NumRects,
                                               const D3D12_RECT *pRects) = 0;
    virtual void STDCALL ClearUnorderedAccessViewUint(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
                                                      D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, ID3D12Resource *pResource,
                                                      const u32 Values[4], u32 NumRects, const D3D12_RECT *pRects) = 0;
    virtual void STDCALL ClearUnorderedAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
                                                       D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle, ID3D12Resource *pResource,
                                                       const f32 Values[ 4 ], u32 NumRects, const D3D12_RECT *pRects) = 0;
    virtual void STDCALL DiscardResource(ID3D12Resource *pResource, const D3D12_DISCARD_REGION *pRegion) = 0;
    virtual void STDCALL BeginQuery(ID3D12QueryHeap *pQueryHeap, D3D12_QUERY_TYPE Type, u32 Index) = 0;
    virtual void STDCALL EndQuery(ID3D12QueryHeap *pQueryHeap, D3D12_QUERY_TYPE Type, u32 Index) = 0;
    virtual void STDCALL ResolveQueryData(ID3D12QueryHeap *pQueryHeap, D3D12_QUERY_TYPE Type, u32 StartIndex, u32 NumQueries,
                                          ID3D12Resource *pDestinationBuffer,
                                          u64 AlignedDestinationBufferOffset) = 0;
    virtual void STDCALL SetPredication(ID3D12Resource *pBuffer, u64 AlignedBufferOffset, D3D12_PREDICATION_OP Operation) = 0;
    virtual void STDCALL SetMarker(u32 Metadata, const void *pData, u32 Size) = 0;
    virtual void STDCALL BeginEvent(u32 Metadata, const void *pData, u32 Size) = 0;
    virtual void STDCALL EndEvent() = 0;
    virtual void STDCALL ExecuteIndirect(ID3D12CommandSignature *pCommandSignature, u32 MaxCommandCount, ID3D12Resource *pArgumentBuffer,
                                         u64 ArgumentBufferOffset, ID3D12Resource *pCountBuffer, u64 CountBufferOffset) = 0;
};

struct ID3D12CommandQueue : public ID3D12Pageable
{
    virtual void                     STDCALL UpdateTileMappings(ID3D12Resource *pResource, u32 NumResourceRegions,
                                                                const D3D12_TILED_RESOURCE_COORDINATE *pResourceRegionStartCoordinates,
                                                                const D3D12_TILE_REGION_SIZE *pResourceRegionSizes, ID3D12Heap *pHeap,
                                                                u32 NumRanges, const D3D12_TILE_RANGE_FLAGS *pRangeFlags,
                                                                const u32 *pHeapRangeStartOffsets, const u32 *pRangeTileCounts,
                                                                D3D12_TILE_MAPPING_FLAGS Flags) = 0;
    virtual void                     STDCALL CopyTileMappings(ID3D12Resource *pDstResource,
                                                              const D3D12_TILED_RESOURCE_COORDINATE *pDstRegionStartCoordinate,
                                                              ID3D12Resource *pSrcResource,
                                                              const D3D12_TILED_RESOURCE_COORDINATE *pSrcRegionStartCoordinate,
                                                              const D3D12_TILE_REGION_SIZE *pRegionSize,
                                                              D3D12_TILE_MAPPING_FLAGS Flags) = 0;
    virtual void                     STDCALL ExecuteCommandLists(u32 NumCommandLists, ID3D12CommandList *const *ppCommandLists) = 0;
    virtual void                     STDCALL SetMarker(u32 Metadata, const void *pData, u32 Size) = 0;
    virtual void                     STDCALL BeginEvent(u32 Metadata, const void *pData, u32 Size) = 0;
    virtual void                     STDCALL EndEvent() = 0;
    virtual i32                      STDCALL Signal(ID3D12Fence *pFence, u64 Value) = 0;
    virtual i32                      STDCALL Wait(ID3D12Fence *pFence, u64 Value) = 0;
    virtual i32                      STDCALL GetTimestampFrequency(u64 *pFrequency) = 0;
    virtual i32                      STDCALL GetClockCalibration(u64 *pGpuTimestamp, u64 *pCpuTimestamp) = 0;
    virtual D3D12_COMMAND_QUEUE_DESC STDCALL GetDesc() = 0;
};

struct ID3D12Device : public ID3D12Object
{
    virtual u32                            STDCALL GetNodeCount() = 0;
    virtual i32                            STDCALL CreateCommandQueue(const D3D12_COMMAND_QUEUE_DESC *pDesc, const GUID &riid,
                                                                      void **ppCommandQueue) = 0;
    virtual i32                            STDCALL CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type, const GUID &riid,
                                                                          void **ppCommandAllocator) = 0;
    virtual i32                            STDCALL CreateGraphicsPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC *pDesc,
                                                                               const GUID &riid,
                                                                               void **ppPipelineState) = 0;
    virtual i32                            STDCALL CreateComputePipelineState(const D3D12_COMPUTE_PIPELINE_STATE_DESC *pDesc,
                                                                              const GUID &riid,
                                                                              void **ppPipelineState) = 0;
    virtual i32                            STDCALL CreateCommandList(u32 nodeMask, D3D12_COMMAND_LIST_TYPE type,
                                                                     ID3D12CommandAllocator *pCommandAllocator,
                                                                     ID3D12PipelineState *pInitialState, const GUID &riid,
                                                                     void **ppCommandList) = 0;
    virtual i32                            STDCALL CheckFeatureSupport(D3D12_FEATURE Feature, void *pFeatureSupportData,
                                                                       u32 FeatureSupportDataSize) = 0;
    virtual i32                            STDCALL CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC *pDescriptorHeapDesc,
                                                                        const GUID &riid, void **ppvHeap) = 0;
    virtual u32                            STDCALL GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType) = 0;
    virtual i32                            STDCALL CreateRootSignature(u32 nodeMask, const void *pBlobWithRootSignature,
                                                                       u64 blobLengthInBytes, const GUID &riid,
                                                                       void **ppvRootSignature) = 0;
    virtual void                           STDCALL CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC *pDesc,
                                                                            D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
    virtual void                           STDCALL CreateShaderResourceView(ID3D12Resource *pResource,
                                                                            const D3D12_SHADER_RESOURCE_VIEW_DESC *pDesc,
                                                                            D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
    virtual void                           STDCALL CreateUnorderedAccessView(ID3D12Resource *pResource, ID3D12Resource *pCounterResource,
                                                                             const D3D12_UNORDERED_ACCESS_VIEW_DESC *pDesc,
                                                                             D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
    virtual void                           STDCALL CreateRenderTargetView(ID3D12Resource *pResource,
                                                                          const D3D12_RENDER_TARGET_VIEW_DESC *pDesc,
                                                                          D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
    virtual void                           STDCALL CreateDepthStencilView(ID3D12Resource *pResource,
                                                                          const D3D12_DEPTH_STENCIL_VIEW_DESC *pDesc,
                                                                          D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
    virtual void                           STDCALL CreateSampler(const D3D12_SAMPLER_DESC *pDesc,
                                                                 D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) = 0;
    virtual void                           STDCALL CopyDescriptors(u32 NumDestDescriptorRanges,
                                                                   const D3D12_CPU_DESCRIPTOR_HANDLE *pDestDescriptorRangeStarts,
                                                                   const u32 *pDestDescriptorRangeSizes, u32 NumSrcDescriptorRanges,
                                                                   const D3D12_CPU_DESCRIPTOR_HANDLE *pSrcDescriptorRangeStarts,
                                                                   const u32 *pSrcDescriptorRangeSizes,
                                                                   D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) = 0;
    virtual void                           STDCALL CopyDescriptorsSimple(u32 NumDescriptors,
                                                                         D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
                                                                         D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
                                                                         D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType) = 0;
    virtual D3D12_RESOURCE_ALLOCATION_INFO STDCALL GetResourceAllocationInfo(u32 visibleMask, u32 numResourceDescs,
                                                                             const D3D12_RESOURCE_DESC *pResourceDescs) = 0;
    virtual D3D12_HEAP_PROPERTIES          STDCALL GetCustomHeapProperties(u32 nodeMask, D3D12_HEAP_TYPE heapType) = 0;
    virtual i32                            STDCALL CreateCommittedResource(const D3D12_HEAP_PROPERTIES *pHeapProperties,
                                                                           D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC *pDesc,
                                                                           D3D12_RESOURCE_STATES InitialResourceState,
                                                                           const D3D12_CLEAR_VALUE *pOptimizedClearValue,
                                                                           const GUID &riidResource, void **ppvResource) = 0;
    virtual i32                            STDCALL CreateHeap(const D3D12_HEAP_DESC *pDesc, const GUID &riid, void **ppvHeap) = 0;
    virtual i32                            STDCALL CreatePlacedResource(ID3D12Heap *pHeap, u64 HeapOffset, const D3D12_RESOURCE_DESC *pDesc,
                                                                        D3D12_RESOURCE_STATES InitialState,
                                                                        const D3D12_CLEAR_VALUE *pOptimizedClearValue,
                                                                        const GUID &riid, void **ppvResource) = 0;
    virtual i32                            STDCALL CreateReservedResource(const D3D12_RESOURCE_DESC *pDesc,
                                                                          D3D12_RESOURCE_STATES InitialState,
                                                                          const D3D12_CLEAR_VALUE *pOptimizedClearValue,
                                                                          const GUID &riid, void **ppvResource) = 0;
    virtual i32                            STDCALL CreateSharedHandle(ID3D12DeviceChild *pObject, const SECURITY_ATTRIBUTES *pAttributes,
                                                                      u32 Access, wchar_t *Name, void **pHandle) = 0;
    virtual i32                            STDCALL OpenSharedHandle(void *NTHandle, const GUID &riid, void **ppvObj) = 0;
    virtual i32                            STDCALL OpenSharedHandleByName(wchar_t *Name, u32 Access, void **pNTHandle) = 0;
    virtual i32                            STDCALL MakeResident(u32 NumObjects, ID3D12Pageable *const *ppObjects) = 0;
    virtual i32                            STDCALL Evict(u32 NumObjects, ID3D12Pageable *const *ppObjects) = 0;
    virtual i32                            STDCALL CreateFence(u64 InitialValue, D3D12_FENCE_FLAGS Flags, const GUID &riid,
                                                               void **ppFence) = 0;
    virtual i32                            STDCALL GetDeviceRemovedReason() = 0;
    virtual void                           STDCALL GetCopyableFootprints(const D3D12_RESOURCE_DESC *pResourceDesc, u32 FirstSubresource,
                                                                         u32 NumSubresources, u64 BaseOffset,
                                                                         D3D12_PLACED_SUBRESOURCE_FOOTPRINT *pLayouts, u32 *pNumRows,
                                                                         u64 *pRowSizeInBytes, u64 *pTotalBytes) = 0;
    virtual i32                            STDCALL CreateQueryHeap(const D3D12_QUERY_HEAP_DESC *pDesc, const GUID &riid, void **ppvHeap) = 0;
    virtual i32                            STDCALL SetStablePowerState(b32 Enable) = 0;
    virtual i32                            STDCALL CreateCommandSignature(const D3D12_COMMAND_SIGNATURE_DESC *pDesc, ID3D12RootSignature *pRootSignature,
                                                                          const GUID &riid, void **ppvCommandSignature) = 0;
    virtual void                           STDCALL GetResourceTiling(ID3D12Resource *pTiledResource, u32 *pNumTilesForEntireResource,
                                                                     D3D12_PACKED_MIP_INFO *pPackedMipDesc,
                                                                     D3D12_TILE_SHAPE *pStandardTileShapeForNonPackedMips,
                                                                     u32 *pNumSubresourceTilings, u32 FirstSubresourceTilingToGet,
                                                                     D3D12_SUBRESOURCE_TILING *pSubresourceTilingsForNonPackedMips) = 0;
    virtual LUID                           STDCALL GetAdapterLuid() = 0;
};


struct ID3D12Debug : public IUnknown
{
    virtual void STDCALL EnableDebugLayer() = 0;
};


struct DXGI_RGB
{
    f32 Red;
    f32 Green;
    f32 Blue;
};

struct DXGI_RGBA
{
    f32 Red;
    f32 Green;
    f32 Blue;
    f32 Alpha;
};

struct DXGI_GAMMA_CONTROL
{
    DXGI_RGB Scale;
    DXGI_RGB Offset;
    DXGI_RGB GammaCurve[1025];
};

struct DXGI_GAMMA_CONTROL_CAPABILITIES
{
    b32 ScaleAndOffsetSupported;
    f32 MaxConvertedValue;
    f32 MinConvertedValue;
    u32 NumGammaControlPoints;
    f32 ControlPointPositions[1025];
};

struct DXGI_RATIONAL
{
    u32 Numerator;
    u32 Denominator;
};

enum DXGI_MODE_SCANLINE_ORDER
{
    DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED       = 0,
    DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE       = 1,
    DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
    DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
};

enum DXGI_MODE_SCALING
{
    DXGI_MODE_SCALING_UNSPECIFIED = 0,
    DXGI_MODE_SCALING_CENTERED    = 1,
    DXGI_MODE_SCALING_STRETCHED   = 2
};

enum DXGI_MODE_ROTATION
{
    DXGI_MODE_ROTATION_UNSPECIFIED = 0,
    DXGI_MODE_ROTATION_IDENTITY    = 1,
    DXGI_MODE_ROTATION_ROTATE90    = 2,
    DXGI_MODE_ROTATION_ROTATE180   = 3,
    DXGI_MODE_ROTATION_ROTATE270   = 4
};

struct DXGI_MODE_DESC
{
    u32                      Width;
    u32                      Height;
    DXGI_RATIONAL            RefreshRate;
    DXGI_FORMAT              Format;
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
    DXGI_MODE_SCALING        Scaling;
};

struct DXGI_OUTPUT_DESC
{
    wchar_t            DeviceName[32];
    RECT               DesktopCoordinates;
    b32                AttachedToDesktop;
    DXGI_MODE_ROTATION Rotation;
    void               *Monitor;
};

struct DXGI_SURFACE_DESC
{
    u32              Width;
    u32              Height;
    DXGI_FORMAT      Format;
    DXGI_SAMPLE_DESC SampleDesc;
};

struct DXGI_MAPPED_RECT
{
    i32 Pitch;
    u8  *pBits;
};

struct DXGI_FRAME_STATISTICS
{
    u32 PresentCount;
    u32 PresentRefreshCount;
    u32 SyncRefreshCount;
    i64 SyncQPCTime;
    i64 SyncGPUTime;
};

struct DXGI_ADAPTER_DESC
{
    wchar_t Description[128];
    u32     VendorId;
    u32     DeviceId;
    u32     SubSysId;
    u32     Revision;
    u64     DedicatedVideoMemory;
    u64     DedicatedSystemMemory;
    u64     SharedSystemMemory;
    LUID    AdapterLuid;
};

struct DXGI_ADAPTER_DESC1
{
    wchar_t Description[128];
    u32     VendorId;
    u32     DeviceId;
    u32     SubSysId;
    u32     Revision;
    u64     DedicatedVideoMemory;
    u64     DedicatedSystemMemory;
    u64     SharedSystemMemory;
    LUID    AdapterLuid;
    u32     Flags;
};

#define DXGI_USAGE_SHADER_INPUT         0x00000010UL
#define DXGI_USAGE_RENDER_TARGET_OUTPUT 0x00000020UL
#define DXGI_USAGE_BACK_BUFFER          0x00000040UL
#define DXGI_USAGE_SHARED               0x00000080UL
#define DXGI_USAGE_READ_ONLY            0x00000100UL
#define DXGI_USAGE_DISCARD_ON_PRESENT   0x00000200UL
#define DXGI_USAGE_UNORDERED_ACCESS     0x00000400UL
typedef u32 DXGI_USAGE;

enum DXGI_SWAP_EFFECT
{
    DXGI_SWAP_EFFECT_DISCARD         = 0,
    DXGI_SWAP_EFFECT_SEQUENTIAL      = 1,
    DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL = 3,
    DXGI_SWAP_EFFECT_FLIP_DISCARD    = 4
};

struct DXGI_SWAP_CHAIN_DESC
{
    DXGI_MODE_DESC   BufferDesc;
    DXGI_SAMPLE_DESC SampleDesc;
    DXGI_USAGE       BufferUsage;
    u32              BufferCount;
    void             *OutputWindow;
    b32              Windowed;
    DXGI_SWAP_EFFECT SwapEffect;
    u32              Flags;
};

enum DXGI_SCALING
{
    DXGI_SCALING_STRETCH              = 0,
    DXGI_SCALING_NONE                 = 1,
    DXGI_SCALING_ASPECT_RATIO_STRETCH = 2
};

enum DXGI_ALPHA_MODE
{
    DXGI_ALPHA_MODE_UNSPECIFIED   = 0,
    DXGI_ALPHA_MODE_PREMULTIPLIED = 1,
    DXGI_ALPHA_MODE_STRAIGHT	  = 2,
    DXGI_ALPHA_MODE_IGNORE        = 3,
    DXGI_ALPHA_MODE_FORCE_DWORD   = 0xffffffff
};

struct DXGI_SWAP_CHAIN_DESC1
{
    u32              Width;
    u32              Height;
    DXGI_FORMAT      Format;
    b32              Stereo;
    DXGI_SAMPLE_DESC SampleDesc;
    DXGI_USAGE       BufferUsage;
    u32              BufferCount;
    DXGI_SCALING     Scaling;
    DXGI_SWAP_EFFECT SwapEffect;
    DXGI_ALPHA_MODE  AlphaMode;
    u32              Flags;
};

struct DXGI_SWAP_CHAIN_FULLSCREEN_DESC
{
    DXGI_RATIONAL            RefreshRate;
    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
    DXGI_MODE_SCALING        Scaling;
    b32                      Windowed;
};

struct DXGI_PRESENT_PARAMETERS
{
    u32   DirtyRectsCount;
    RECT  *pDirtyRects;
    RECT  *pScrollRect;
    POINT *pScrollOffset;
};

struct DXGI_MATRIX_3X2_F
{
    f32 _11;
    f32 _12;
    f32 _21;
    f32 _22;
    f32 _31;
    f32 _32;
};

enum DXGI_COLOR_SPACE_TYPE
{
    DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709           = 0,
    DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709           = 1,
    DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P709         = 2,
    DXGI_COLOR_SPACE_RGB_STUDIO_G22_NONE_P2020        = 3,
    DXGI_COLOR_SPACE_RESERVED                         = 4,
    DXGI_COLOR_SPACE_YCBCR_FULL_G22_NONE_P709_X601    = 5,
    DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P601       = 6,
    DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P601         = 7,
    DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P709       = 8,
    DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P709         = 9,
    DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_LEFT_P2020      = 10,
    DXGI_COLOR_SPACE_YCBCR_FULL_G22_LEFT_P2020        = 11,
    DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020        = 12,
    DXGI_COLOR_SPACE_YCBCR_STUDIO_G2084_LEFT_P2020    = 13,
    DXGI_COLOR_SPACE_RGB_STUDIO_G2084_NONE_P2020      = 14,
    DXGI_COLOR_SPACE_YCBCR_STUDIO_G22_TOPLEFT_P2020   = 15,
    DXGI_COLOR_SPACE_YCBCR_STUDIO_G2084_TOPLEFT_P2020 = 16,
    DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P2020          = 17,
    DXGI_COLOR_SPACE_CUSTOM                           = 0xFFFFFFFF
};

struct IDXGIObject : public IUnknown
{
    virtual i32 STDCALL SetPrivateData(const GUID &Name, u32 DataSize, const void *pData) = 0;
    virtual i32 STDCALL SetPrivateDataInterface(const GUID &Name, const IUnknown *pUnknown) = 0;
    virtual i32 STDCALL GetPrivateData(const GUID &Name, u32 *pDataSize, void *pData) = 0;
    virtual i32 STDCALL GetParent(const GUID &riid, void **ppParent) = 0;
};

struct IDXGIDeviceSubObject : public IDXGIObject
{
    virtual i32 STDCALL GetDevice(const GUID &riid, void **ppDevice) = 0;
};

struct IDXGISurface : public IDXGIDeviceSubObject
{
    virtual i32 STDCALL GetDesc(DXGI_SURFACE_DESC *pDesc) = 0;
    virtual i32 STDCALL Map(DXGI_MAPPED_RECT *pLockedRect, u32 MapFlags) = 0;
    virtual i32 STDCALL Unmap() = 0;
};

struct IDXGIOutput : public IDXGIObject
{
    virtual i32  STDCALL GetDesc(DXGI_OUTPUT_DESC *pDesc) = 0;
    virtual i32  STDCALL GetDisplayModeList(DXGI_FORMAT EnumFormat, u32 Flags,
                                            u32 *pNumModes,
                                            DXGI_MODE_DESC *pDesc) = 0;
    virtual i32  STDCALL FindClosestMatchingMode(const DXGI_MODE_DESC *pModeToMatch,
                                                 DXGI_MODE_DESC *pClosestMatch,
                                                 IUnknown *pConcernedDevice) = 0;
    virtual  i32 STDCALL WaitForVBlank() = 0;
    virtual  i32 STDCALL TakeOwnership(IUnknown *pDevice, b32 Exclusive) = 0;
    virtual void STDCALL ReleaseOwnership() = 0;
    virtual  i32 STDCALL GetGammaControlCapabilities(DXGI_GAMMA_CONTROL_CAPABILITIES *pGammaCaps) = 0;
    virtual  i32 STDCALL SetGammaControl(const DXGI_GAMMA_CONTROL *pArray) = 0;
    virtual  i32 STDCALL GetGammaControl(DXGI_GAMMA_CONTROL *pArray) = 0;
    virtual  i32 STDCALL SetDisplaySurface(IDXGISurface *pScanoutSurface) = 0;
    virtual  i32 STDCALL GetDisplaySurfaceData(IDXGISurface *pDestination) = 0;
    virtual  i32 STDCALL GetFrameStatistics(DXGI_FRAME_STATISTICS *pStats) = 0;
};

struct IDXGIAdapter : public IDXGIObject
{
    virtual i32 STDCALL EnumOutputs(u32 Output, IDXGIOutput **ppOutput) = 0;
    virtual i32 STDCALL GetDesc(DXGI_ADAPTER_DESC *pDesc) = 0;
    virtual i32 STDCALL CheckInterfaceSupport(const GUID &InterfaceName, i64 *pUMDVersion) = 0;
};

struct IDXGIAdapter1 : public IDXGIAdapter
{
    virtual i32 STDCALL GetDesc1(DXGI_ADAPTER_DESC1 *pDesc) = 0;
};

struct IDXGISwapChain : public IDXGIDeviceSubObject
{
    virtual i32 STDCALL Present(u32 SyncInterval, u32 Flags) = 0;
    virtual i32 STDCALL GetBuffer(u32 Buffer, const GUID &riid, void **ppSurface) = 0;
    virtual i32 STDCALL SetFullscreenState(b32 Fullscreen, IDXGIOutput *pTarget) = 0;
    virtual i32 STDCALL GetFullscreenState(b32 *pFullscreen, IDXGIOutput **ppTarget) = 0;
    virtual i32 STDCALL GetDesc(DXGI_SWAP_CHAIN_DESC *pDesc) = 0; 
    virtual i32 STDCALL ResizeBuffers(u32 BufferCount, u32 Width, u32 Height, DXGI_FORMAT NewFormat,
                                      u32 SwapChainFlags) = 0;
    virtual i32 STDCALL ResizeTarget(const DXGI_MODE_DESC *pNewTargetParameters) = 0;
    virtual i32 STDCALL GetContainingOutput(IDXGIOutput **ppOutput) = 0;
    virtual i32 STDCALL GetFrameStatistics(DXGI_FRAME_STATISTICS *pStats) = 0;
    virtual i32 STDCALL GetLastPresentCount(u32 *pLastPresentCount) = 0;
};

struct IDXGISwapChain1 : public IDXGISwapChain
{
    virtual i32 STDCALL GetDesc1(DXGI_SWAP_CHAIN_DESC1 *pDesc) = 0;
    virtual i32 STDCALL GetFullscreenDesc(DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pDesc) = 0;
    virtual i32 STDCALL GetHwnd(void **pHwnd) = 0;
    virtual i32 STDCALL GetCoreWindow(const GUID &refiid, void **ppUnk) = 0;
    virtual i32 STDCALL Present1(u32 SyncInterval, u32 PresentFlags,
                                 const DXGI_PRESENT_PARAMETERS *pPresentParameters) = 0;
    virtual b32 STDCALL IsTemporaryMonoSupported() = 0;
    virtual i32 STDCALL GetRestrictToOutput(IDXGIOutput **ppRestrictToOutput) = 0;
    virtual i32 STDCALL SetBackgroundColor(const DXGI_RGBA *pColor) = 0;
    virtual i32 STDCALL GetBackgroundColor(DXGI_RGBA *pColor) = 0;
    virtual i32 STDCALL SetRotation(DXGI_MODE_ROTATION Rotation) = 0;
    virtual i32 STDCALL GetRotation(DXGI_MODE_ROTATION *pRotation) = 0;
};

struct IDXGISwapChain2 : public IDXGISwapChain1
{
    virtual i32    STDCALL SetSourceSize(u32 Width, u32 Height) = 0;
    virtual i32    STDCALL GetSourceSize(u32 *pWidth, u32 *pHeight) = 0;
    virtual i32    STDCALL SetMaximumFrameLatency(u32 MaxLatency) = 0;
    virtual i32    STDCALL GetMaximumFrameLatency(u32 *pMaxLatency) = 0;
    virtual void * STDCALL GetFrameLatencyWaitableObject() = 0;
    virtual i32    STDCALL SetMatrixTransform(const DXGI_MATRIX_3X2_F *pMatrix) = 0;
    virtual i32    STDCALL GetMatrixTransform(DXGI_MATRIX_3X2_F *pMatrix) = 0;
};

struct IDXGISwapChain3 : public IDXGISwapChain2
{
    virtual u32 STDCALL GetCurrentBackBufferIndex() = 0;
    virtual i32 STDCALL CheckColorSpaceSupport(DXGI_COLOR_SPACE_TYPE ColorSpace, u32 *pColorSpaceSupport) = 0;
    virtual i32 STDCALL SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace) = 0;
    virtual i32 STDCALL ResizeBuffers1(u32 BufferCount, u32 Width, u32 Height, DXGI_FORMAT Format, u32 SwapChainFlags,
                                       const u32 *pCreationNodeMask, IUnknown *const *ppPresentQueue) = 0;
};

struct IDXGIFactory : public IDXGIObject
{
    virtual i32 STDCALL EnumAdapters(u32 Adapter, IDXGIAdapter **ppAdapter) = 0;
    virtual i32 STDCALL MakeWindowAssociation(void *WindowHandle, u32 Flags) = 0;
    virtual i32 STDCALL GetWindowAssociation(void **pWindowHandle) = 0;
    virtual i32 STDCALL CreateSwapChain(IUnknown *pDevice, DXGI_SWAP_CHAIN_DESC *pDesc, IDXGISwapChain **ppSwapChain) = 0;
    virtual i32 STDCALL CreateSoftwareAdapter(void *Module, IDXGIAdapter **ppAdapter) = 0;
};

struct IDXGIFactory1 : public IDXGIFactory
{
    virtual i32 STDCALL EnumAdapters1(u32 Adapter, IDXGIAdapter1 **ppAdapter) = 0;
    virtual b32 STDCALL IsCurrent() = 0;
};

struct IDXGIFactory2 : public IDXGIFactory1
{
    virtual b32  STDCALL IsWindowedStereoEnabled() = 0;
    virtual i32  STDCALL CreateSwapChainForHwnd(IUnknown *pDevice, void *hWnd, const DXGI_SWAP_CHAIN_DESC1 *pDesc,
                                                const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
                                                IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain) = 0;
    virtual i32  STDCALL CreateSwapChainForCoreWindow(IUnknown *pDevice, IUnknown *pWindow,
                                                      const DXGI_SWAP_CHAIN_DESC1 *pDesc,
                                                      IDXGIOutput *pRestrictToOutput,
                                                      IDXGISwapChain1 **ppSwapChain) = 0;
    virtual i32  STDCALL GetSharedResourceAdapterLuid(void *hResource, LUID *pLuid) = 0;
    virtual i32  STDCALL RegisterStereoStatusWindow(void *WindowHandle, u32 wMsg, u32 *pdwCookie) = 0;
    virtual i32  STDCALL RegisterStereoStatusEvent(void *hEvent, u32 *pdwCookie) = 0;
    virtual void STDCALL UnregisterStereoStatus(u32 dwCookie) = 0;
    virtual i32  STDCALL RegisterOcclusionStatusWindow(void *WindowHandle, u32 wMsg, u32 *pdwCookie) = 0;
    virtual i32  STDCALL RegisterOcclusionStatusEvent(void *hEvent, u32 *pdwCookie) = 0;
    virtual void STDCALL UnregisterOcclusionStatus(u32 dwCookie) = 0;
    virtual i32  STDCALL CreateSwapChainForComposition(IUnknown *pDevice, const DXGI_SWAP_CHAIN_DESC1 *pDesc,
                                                       IDXGIOutput *pRestrictToOutput, IDXGISwapChain1 **ppSwapChain) = 0;
};

struct IDXGIFactory3 : public IDXGIFactory2
{
    virtual u32 STDCALL GetCreationFlags() = 0;
};

struct IDXGIFactory4 : public IDXGIFactory3
{
    virtual i32 STDCALL EnumAdapterByLuid(LUID AdapterLuid, const GUID &riid, void **ppvAdapter) = 0;
    virtual i32 STDCALL EnumWarpAdapter(const GUID &riid, void **ppvAdapter) = 0;
};


const GUID IID_ID3D12Debug = { 0x344488b7,0x6846,0x474b,0xb9,0x89,0xf0,0x27,0x44,0x82,0x45,0xe0 };
const GUID IID_IDXGISwapChain = { 0x310d36a0,0xd2e7,0x4c0a,0xaa,0x04,0x6a,0x9d,0x23,0xb8,0x88,0x6a };
const GUID IID_IDXGISwapChain3 = { 0x94d99bdb,0xf1f8,0x4ab0,0xb2,0x36,0x7d,0xa0,0x17,0x0e,0xda,0xb1 };
const GUID IID_IDXGIFactory4 = { 0x1bc6ea02,0xef36,0x464f,0xbf,0x0c,0x21,0xca,0x39,0xe5,0x16,0x8a };
const GUID IID_ID3D12GraphicsCommandList = { 0x5b160d0f,0xac1b,0x4185,0x8b,0xa8,0xb3,0xae,0x42,0xa5,0xa4,0x55 };
const GUID IID_ID3D12CommandQueue = { 0x0ec870a6,0x5d7e,0x4c22,0x8c,0xfc,0x5b,0xaa,0xe0,0x76,0x16,0xed };
const GUID IID_ID3D12Device = { 0x189819f1,0x1db6,0x4b57,0xbe,0x54,0x18,0x21,0x33,0x9b,0x85,0xf7 };
const GUID IID_ID3D12DescriptorHeap = { 0x8efb471d,0x616c,0x4f49,0x90,0xf7,0x12,0x7b,0xb7,0x63,0xfa,0x51 };
const GUID IID_ID3D12Resource = { 0x696442be,0xa72e,0x4059,0xbc,0x79,0x5b,0x5c,0x98,0x04,0x0f,0xad };
const GUID IID_ID3D12RootSignature = { 0xc54a6b66,0x72df,0x4ee8,0x8b,0xe5,0xa9,0x46,0xa1,0x42,0x92,0x14 };
const GUID IID_ID3D12CommandAllocator = { 0x6102dee4,0xaf59,0x4b09,0xb9,0x99,0xb4,0x4d,0x73,0xf0,0x9b,0x24 };
const GUID IID_ID3D12Fence = { 0x0a753dcf,0xc4d8,0x4b91,0xad,0xf6,0xbe,0x5a,0x60,0xd9,0x5a,0x76 };
const GUID IID_ID3D12PipelineState = { 0x765a30f3,0xf624,0x4c6f,0xa8,0x28,0xac,0xe9,0x48,0x62,0x24,0x45 };


extern "C" void * STDCALL LoadLibraryA(const char *filename);
extern "C" void * STDCALL GetProcAddress(void *module, const char *procname);

typedef void  (STDCALL *OutputDebugString_fn)(const char *string);
typedef void  (STDCALL *ExitProcess_fn)(u32 exit_code);
typedef void *(STDCALL *GetModuleHandle_fn)(const char *module_name);
typedef void  (STDCALL *Sleep_fn)(u32 milisec);

typedef b32   (STDCALL *PeekMessage_fn)(MSG *msg, void *hwnd, u32 filter_min, u32 filter_max, u32 remove_msg);
typedef i64   (STDCALL *DispatchMessage_fn)(const MSG *msg);
typedef void  (STDCALL *PostQuitMessage_fn)(i32 exit_code);
typedef i64   (STDCALL *DefWindowProc_fn)(void *hwnd, u32 msg, u64 wparam, i64 lparam);
typedef void *(STDCALL *LoadCursor_fn)(void *hinstance, const char *cursor_name);
typedef i16   (STDCALL *RegisterClass_fn)(const WNDCLASS *wndclass);
typedef void *(STDCALL *CreateWindowEx_fn)(u32 ex_style, const char *class_name, const char *window_name,
                                           u32 style, i32 x, i32 y, i32 width, i32 height,
                                           void *hwnd_parent, void *hmenu, void *hinstance, void *param);
typedef b32   (STDCALL *AdjustWindowRect_fn)(RECT *lpRect, u32 dwStyle, b32 bMenu);

typedef i32   (STDCALL *CreateDXGIFactory1_fn)(const GUID &riid, void **ppFactory);

typedef i32 (STDCALL *D3D12CreateDevice_fn)(IUnknown *, D3D_FEATURE_LEVEL, const GUID &, void **);
typedef i32 (STDCALL *D3D12GetDebugInterface_fn)(const GUID &, void **);


static OutputDebugString_fn         OutputDebugString;
static ExitProcess_fn               ExitProcess;
static DefWindowProc_fn             DefWindowProc;
static Sleep_fn                     Sleep;

static PeekMessage_fn               PeekMessage;
static DispatchMessage_fn           DispatchMessage;
static PostQuitMessage_fn           PostQuitMessage;
static GetModuleHandle_fn           GetModuleHandle;
static LoadCursor_fn                LoadCursor;
static RegisterClass_fn             RegisterClass;
static CreateWindowEx_fn            CreateWindowEx;
static AdjustWindowRect_fn          AdjustWindowRect;

static CreateDXGIFactory1_fn        CreateDXGIFactory1;

static D3D12GetDebugInterface_fn    D3D12GetDebugInterface;
static D3D12CreateDevice_fn         D3D12CreateDevice;
