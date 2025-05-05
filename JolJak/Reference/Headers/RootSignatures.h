#pragma once
#include "Base.h"

BEGIN(Engine)

class CRootSignature : public CBase
{
public:
	CRootSignature(ID3D12Device* pDevice);
	virtual ~CRootSignature() = default;

public:
	ID3D12RootSignature* Get() const { return m_RootSignature; }

private:
	/*void Push(D3D12_ROOT_PARAMETER param, D3D12_ROOT_PARAMETER_TYPE paramType, UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility, UINT num32BitValues = 0);
	void PushTable(D3D12_ROOT_PARAMETER param, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT shaderRegister, UINT registerSpace, UINT numDescriptors, D3D12_SHADER_VISIBILITY visibility);*/
		
private:
	//std::vector<D3D12_ROOT_PARAMETER>	mParams{};		// all root parameters
	//std::vector<D3D12_DESCRIPTOR_RANGE> mRanges{};		// all root ranges

private:
	ID3D12RootSignature* m_RootSignature = {};

public:
	static CRootSignature* Create(ID3D12Device* pDevice);
	void Free() override;
};

END