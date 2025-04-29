#include "Component.h"

CComponent::CComponent(ID3D12Device* pDevice) : m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

CComponent::CComponent(const CComponent& rhs) : m_pDevice(rhs.m_pDevice)
{
	Safe_AddRef(m_pDevice);
}

void CComponent::Free()
{
	Safe_Release(m_pDevice);
}
