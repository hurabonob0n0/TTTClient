#pragma once
#include "Base.h"
#include "PSO.h"

BEGIN(Engine)

class CPSOMgr: public CBase
{
	DECLARE_SINGLETON(CPSOMgr)

public:
	CPSOMgr() = default;
	virtual ~CPSOMgr() = default;

public:
	// PSO �߰�
	HRESULT AddPSO(const string& PSOName, CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice);

	// PSO ���� ���
	CPSO* GetPSOObj(const string& PSOName) const;

	// PSO ���� ����
	ID3D12PipelineState* Get(const string& PSOName) const;
	//SIZE_T GetBufferSize(const string& shaderName) const;

private:
	unordered_map<string, CPSO*> m_PSOs;

public:
	virtual void Free() override;
};

END