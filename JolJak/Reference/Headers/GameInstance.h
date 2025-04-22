#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(HWND windowHandle);

public:
	void Draw();
	void OnResize();

private:
	class CGraphic_Device* m_pGraphic_Device;

public:
	virtual void Free() override;
};

END