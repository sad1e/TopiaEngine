/** This RHI module only includes DirectX12 API for now. */

#pragma once

#include "RHIForwardDecl.h"

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL		( (D3D12_GPU_VIRTUAL_ADDRESS)  0 )
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN	( (D3D12_GPU_VIRTUAL_ADDRESS) -1 )

namespace topia
{
	struct FGfxSettings
	{
		HWND WindowHandle;
		u32 WindowHeight;
		u32 WindowWidth;

		bool bEnableDebug;
		bool bEnableTearing;
	};

	extern FGfxSettings GfxSettings;

	extern void InitializeGfxSettings(HWND Handle, u32 Width, u32 Height, bool bDebug, bool bTearing);

	struct FGfxContext
	{
		TRefCountPtr<ID3D12Device>  Device;
		TRefCountPtr<ID3D12Device1> Device1;
		TRefCountPtr<ID3D12Device2> Device2;

		TRefCountPtr<ID3D12CommandQueue> GraphicsCommandQueue;
		TRefCountPtr<ID3D12CommandQueue> ComputeCommandQueue;
		TRefCountPtr<ID3D12CommandQueue> CopyCommandQueue;

		TRefCountPtr<ID3D12CommandSignature> DrawIndirectSignature;
		TRefCountPtr<ID3D12CommandSignature> DispatchIndirectSignature;

		TRefCountPtr<ID3D12QueryHeap> TimerQueryHeap;
		// TRefCountPtr<FBuffer> TimerQueryResolveBuffer;
	};

	struct FVertexBuffer
	{
		// buffer ptr
	};

	struct FIndexBuffer
	{
		// buffer ptr
	};

	struct FPipelineStateObjectDesc
	{
		// input layout
		// root signature, d3d12
		// vs, ps, hs, ds, etc
		// rasterizate state
		// blend state
		// sample mask
		// primitive topology type
		// render target nums
		// rtv format
	};

	struct FPipelineStateObject
	{
		// pso ptr

		FPipelineStateObjectDesc Desc;
	};

	struct FRenderTarget
	{

	};

	extern FGfxContext GfxContext;

	extern void Initialize_RHI();

} // namespace topia::gfx
