#include "BaseGeo.h"
#include "GameInstance.h"
#include "Graphic_Device.h"
#include "FrameResource.h"
#include "Transform.h"
#include "Renderer.h"

CBaseGeo::CBaseGeo(): CRenderObject()
{
}

HRESULT CBaseGeo::Initialize()
{
	__super::Initialize();
	m_TransformCom->Set_State(CTransform::STATE_POSITION, { 0.f,0.f,10.f,0.f });

	m_VIBufferCom = (CVIBuffer_Geos*)m_GameInstance->GetComponent("BaseGeo", nullptr);

	return S_OK;
}

void CBaseGeo::Tick(float fTimeDelta)
{
}

void CBaseGeo::LateTick(float fTimeDelta)
{

}

void CBaseGeo::Render()
{
	/*UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	auto objectCB = CGameInstance::Get_Instance()->m_CurrFrameResource->m_ObjectCB->Resource();

	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress();
	objCBAddress += m_objCBIndex * objCBByteSize;

	CGameInstance::Get_Instance()->m_pGraphic_Device->Get_CommandList()->SetGraphicsRootConstantBufferView(0, objCBAddress);*/

	__super::Render();
}

CBaseGeo* CBaseGeo::Create()
{
	CBaseGeo* pInstance = new CBaseGeo();
	pInstance->Initialize();
	return pInstance;
}

void CBaseGeo::Free()
{
}
