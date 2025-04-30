#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Geos final : public CVIBuffer
{
protected:
	CVIBuffer_Geos(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList);
	CVIBuffer_Geos(CVIBuffer_Geos& rhs);
	virtual~CVIBuffer_Geos() = default;

protected:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Geos* Create(ID3D12Device* pDevice);
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END