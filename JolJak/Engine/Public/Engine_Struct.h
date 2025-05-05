#pragma once
#include "Engine_Macro.h"
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine 
{
    struct ObjectConstants
    {
        DirectX::XMFLOAT4X4 World = DirectX::XMFLOAT4X4{};
        XMFLOAT4            Color = XMFLOAT4{ 1.f,1.f,1.f,1.f};
    };

    struct PassConstants
    {
        DirectX::XMFLOAT4X4 View = DirectX::XMFLOAT4X4{};
        DirectX::XMFLOAT4X4 InvView = DirectX::XMFLOAT4X4{};
        DirectX::XMFLOAT4X4 Proj = DirectX::XMFLOAT4X4{};
        DirectX::XMFLOAT4X4 InvProj = DirectX::XMFLOAT4X4{};
        DirectX::XMFLOAT4X4 ViewProj = DirectX::XMFLOAT4X4{};
        DirectX::XMFLOAT4X4 InvViewProj = DirectX::XMFLOAT4X4{};
        DirectX::XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
        float cbPerObjectPad1 = 0.0f;
        DirectX::XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
        DirectX::XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
        float NearZ = 0.0f;
        float FarZ = 0.0f;
        float TotalTime = 0.0f;
        float DeltaTime = 0.0f;
    };
}


namespace Engine
{
	typedef struct ENGINE_DLL tagVertex_Position
	{
		XMFLOAT3		vPosition;

		static const unsigned int iNumElements = 1;
		static const D3D12_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Normal
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;

		static const unsigned int iNumElements = 2;
		static const D3D12_INPUT_ELEMENT_DESC	VertexElements[iNumElements];

	}VTXPOSNOR;
}