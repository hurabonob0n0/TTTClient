#include "PSOMgr.h"

IMPLEMENT_SINGLETON(CPSOMgr)

HRESULT CPSOMgr::AddPSO(const string& PSOName, CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice)
{
	if (m_PSOs.find(PSOName) != m_PSOs.end())
		return E_FAIL; // 이미 있음

	CPSO* pso = CPSO::Create(pRootSignature, VS, PS,pGD,pDevice);

	if (!pso)
	{
		Safe_Release(pso);
		return E_FAIL;
	}

	m_PSOs[PSOName] = pso;
	return S_OK;
}

CPSO* CPSOMgr::GetPSOObj(const string& PSOName) const
{
	auto iter = m_PSOs.find(PSOName);
	if (iter != m_PSOs.end())
		return iter->second;
	return nullptr;
}

ID3D12PipelineState* CPSOMgr::Get(const string& PSOName) const
{
	CPSO* pso = GetPSOObj(PSOName);
	if (pso)
		return pso->Get();
	return nullptr;
}

void CPSOMgr::Free()
{
	for (auto& pair : m_PSOs)
	{
		Safe_Release(pair.second); // 혹은 delete pair.second;
	}
	m_PSOs.clear();
}
