#include <RHI.h>

namespace topia::gfx
{
    static constexpr u32 BACK_BUFFER_SIZE = 2;

    FGfxSettings GfxSettings;
    void InitializeGfxSettings(HWND Handle, u32 Width, u32 Height, bool bDebug, bool bTearing)
    {
        GfxSettings.WindowHandle = Handle;
        GfxSettings.WindowWidth = Width;
        GfxSettings.WindowHeight = Height;

        GfxSettings.bEnableDebug = bDebug;
        GfxSettings.bEnableTearing = bTearing;
    }

    FGfxContext GfxContext;

    TRefCountPtr<IDXGIFactory4> g_DXGIFactory4;
    TRefCountPtr<IDXGIAdapter4> g_DXGIAdapter4;
    TRefCountPtr<IDXGISwapChain4> g_DXGISwapChain4;

    void Initialize_RHI()
    {
#if defined(_DEBUG)
        TRefCountPtr<ID3D12Debug> DebugInterface;
        ASSERT_SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugInterface)));
        DebugInterface->EnableDebugLayer();
#endif

        UINT CreateFactoryFlags = 0;
#if defined(_DEBUG)
        CreateFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

        ASSERT_SUCCEEDED(CreateDXGIFactory2(CreateFactoryFlags, IID_PPV_ARGS(&g_DXGIFactory4)));

        // Enumrate Adapters.
        {
            u64 MaxDedicatedMem = 0;
            TRefCountPtr<IDXGIAdapter1> pDXGIAdapter1;
            for (u32 i = 0; g_DXGIFactory4->EnumAdapters1(i, &pDXGIAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
            {
                DXGI_ADAPTER_DESC1 AdapterDesc1;
                pDXGIAdapter1->GetDesc1(&AdapterDesc1);

                if ((AdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && SUCCEEDED(D3D12CreateDevice(pDXGIAdapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
                    AdapterDesc1.DedicatedVideoMemory > MaxDedicatedMem)
                {
                    MaxDedicatedMem = AdapterDesc1.DedicatedVideoMemory;
                    ASSERT_SUCCEEDED(pDXGIAdapter1->QueryInterface(IID_PPV_ARGS(&g_DXGIAdapter4)))
                }
            }
        }

        // Create DirectX12 Device.
        {
            ASSERT_SUCCEEDED(D3D12CreateDevice(g_DXGIAdapter4.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&GfxContext.Device)))

#if defined(_DEBUG)
                TRefCountPtr<ID3D12InfoQueue> pInfoQueue;
            if (SUCCEEDED(GfxContext.Device->QueryInterface(IID_PPV_ARGS(&pInfoQueue))))
            {
                pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
                pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
                pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

                D3D12_MESSAGE_SEVERITY Severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

                D3D12_MESSAGE_ID DenyIds[] = { D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE, D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE, D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE };

                D3D12_INFO_QUEUE_FILTER InfoQueueFilter = {};
                InfoQueueFilter.DenyList.NumSeverities = _countof(Severities);
                InfoQueueFilter.DenyList.pSeverityList = Severities;
                InfoQueueFilter.DenyList.NumIDs = _countof(DenyIds);
                InfoQueueFilter.DenyList.pIDList = DenyIds;

                ASSERT_SUCCEEDED(pInfoQueue->PushStorageFilter(&InfoQueueFilter));
            }
#endif
        }

        // Create Command Queues.
        {
            D3D12_COMMAND_QUEUE_DESC CommandQueueDesc;

            /** Graphics Command Queue. */
            CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            CommandQueueDesc.NodeMask = 0;
            CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
            ASSERT_SUCCEEDED(GfxContext.Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&GfxContext.GraphicsCommandQueue)));

            /** Graphics Compute Queue. */
            CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            CommandQueueDesc.NodeMask = 0;
            CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
            ASSERT_SUCCEEDED(GfxContext.Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&GfxContext.ComputeCommandQueue)));

            /** Graphics Copy Queue. */
            CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
            CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            CommandQueueDesc.NodeMask = 0;
            CommandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
            ASSERT_SUCCEEDED(GfxContext.Device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&GfxContext.CopyCommandQueue)));
        }

        // Create Swap Chain.
        {
            TRefCountPtr<IDXGISwapChain1> pSwapChain1;

            DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1 = {};
            SwapChainDesc1.Width = GfxSettings.WindowWidth;
            SwapChainDesc1.Height = GfxSettings.WindowHeight;
            SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            SwapChainDesc1.Stereo = FALSE;
            SwapChainDesc1.SampleDesc = { 1, 0 };
            SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            SwapChainDesc1.BufferCount = BACK_BUFFER_SIZE;
            SwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
            SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            SwapChainDesc1.Flags = GfxSettings.bEnableTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

            ASSERT_SUCCEEDED(g_DXGIFactory4->CreateSwapChainForHwnd(GfxContext.GraphicsCommandQueue.Get(), GfxSettings.WindowHandle, &SwapChainDesc1, nullptr, nullptr, &pSwapChain1))
                ASSERT_SUCCEEDED(g_DXGIFactory4->MakeWindowAssociation(GfxSettings.WindowHandle, DXGI_MWA_NO_ALT_ENTER))
                ASSERT_SUCCEEDED(pSwapChain1->QueryInterface(IID_PPV_ARGS(&g_DXGISwapChain4)))
        }
    }
} // namespace topia
