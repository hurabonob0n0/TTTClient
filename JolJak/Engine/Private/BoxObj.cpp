#include "BoxObj.h"
#include "GameInstance.h"

CBoxObj::CBoxObj() : CRenderObject()
{
}

HRESULT CBoxObj::Initialize()
{
    __super::Initialize();
    m_VIBufferCom = (CVIBuffer_Geos*)m_GameInstance->GetComponent("BaseGeoCom");
    return S_OK;
}

void CBoxObj::Tick(float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CBoxObj::LateTick(float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CBoxObj::Render()
{
    __super::Render();
}

CBoxObj* CBoxObj::Create()
{
    CBoxObj* pInstance = new CBoxObj;
    pInstance->Initialize();
    return pInstance;
}

void CBoxObj::Free()
{
    __super::Free();
}
