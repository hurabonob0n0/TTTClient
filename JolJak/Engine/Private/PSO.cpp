#include "PSO.h"
#include "RootSignatures.h"
#include "Shader.h"
#include "Graphic_Device.h"

CPSO::CPSO()
{
}

void CPSO::Initialize(CRootSignature* pRootSignature,CShader* VS,CShader* PS,CGraphic_Device* pGD,ID3D12Device* pDevice)
{
	VTXPOSNOR inputlayout{};

	ZeroMemory(&m_PsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	m_PsoDesc.InputLayout = { VTXPOSNOR::VertexElements,VTXPOSNOR::iNumElements };
	m_PsoDesc.pRootSignature = pRootSignature->Get();
	m_PsoDesc.VS =
	{
		reinterpret_cast<BYTE*>(VS->Get()),
		VS->Get_Buffer_Size()
	};
	m_PsoDesc.PS =
	{
		reinterpret_cast<BYTE*>(PS->Get()),
		PS->Get_Buffer_Size()
	};
	m_PsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_PsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_PsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_PsoDesc.SampleMask = UINT_MAX;
	m_PsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	m_PsoDesc.NumRenderTargets = 1;
	m_PsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_PsoDesc.SampleDesc.Count = 1;//m4xMsaaState ? 4 : 1;
	m_PsoDesc.SampleDesc.Quality = 0;//m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	m_PsoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;//mDepthStencilFormat;
	//m_PsoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	ThrowIfFailed(pDevice->CreateGraphicsPipelineState(&m_PsoDesc, IID_PPV_ARGS(&m_PSO)));

}

CPSO* CPSO::Create(CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice)
{
	CPSO* pInstance = new CPSO;
	pInstance->Initialize(pRootSignature, VS, PS, pGD, pDevice);
	return pInstance;
}

void CPSO::Free()
{
	Safe_Release(m_PSO);
}
