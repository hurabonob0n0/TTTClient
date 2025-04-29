#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D12Device* pDevice);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) = 0;

protected:
	ID3D12Device* m_pDevice = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END