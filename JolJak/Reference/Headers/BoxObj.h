#pragma once
#include "RenderObject.h"

BEGIN(Engine)

class ENGINE_DLL CBoxObj : public CRenderObject
{
private:
	CBoxObj();
	virtual ~CBoxObj() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(float fTimeDelta);
	virtual void LateTick(float fTimeDelta);
	virtual void Render();

public:
	static CBoxObj* Create();
	virtual void Free() override;
};

END