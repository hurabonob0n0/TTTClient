#include "Transform.h"

CTransform::CTransform() : CComponent()
{
}

CTransform::CTransform(const CTransform& rhs) : CComponent(rhs)
{
}

HRESULT CTransform::Initialize_Prototype()
{
    m_WorldMatrix = XMFLOAT4X4{};
    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    return S_OK;
}


CTransform* CTransform::Create()
{
    CTransform* pInstance = new CTransform();
    pInstance->Initialize_Prototype();
    return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
    CComponent* pInstance = new CTransform(*this);
    pInstance->Initialize(pArg);
    return pInstance;
}

void CTransform::Free()
{
}

