#pragma once
#include "Base.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CGameObject : public CBase
{
public:
	CGameObject();
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(float fTimeDelta) = 0;
	virtual void LateTick(float fTimeDelta) = 0;

protected:
	CTransform* m_TransformCom = {};
	class CGameInstance* m_GameInstance = {};

protected:
	unordered_map<wstring, class CComponent*> m_Components;
	//CComponent* Get_Object_Component(wstring ComName) { return m_Components; };

public:
	void Free() override;
};

END