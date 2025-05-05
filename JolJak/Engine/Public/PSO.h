#pragma once
#include "Base.h"

BEGIN(Engine)

class CRootSignature;
class CShader;
class CGraphic_Device;

class CPSO : public CBase
{
public:
	enum INPUTLAYOUT_TYPE{IT_POS,ITPOSNOR,ITEND};

public:
	CPSO();
	virtual ~CPSO() = default;

public:
	ID3D12PipelineState* Get() const { return m_PSO; }

public:
	void Initialize(CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice);

private:
	ID3D12PipelineState* m_PSO = {};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PsoDesc{};

public:
	static CPSO* Create(CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice);
	void Free() override;

};

END