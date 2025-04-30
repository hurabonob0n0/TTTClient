#pragma once
#include "Engine_Macro.h"
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;

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