#pragma once
#include "GameObject.h"
#include "Renderer.h"

BEGIN(Engine)

class CVIBuffer;

class ENGINE_DLL CRenderObject : public CGameObject
{
public:
	CRenderObject();
	virtual ~CRenderObject() = default;

public:
	virtual void Set_ObjCBIndex(const UINT& index);

public:
	virtual HRESULT Initialize();
	virtual void Tick(float fTimeDelta);
	virtual void LateTick(float fTimeDelta);
	virtual void Render();

protected:
	CVIBuffer* m_VIBufferCom;
	CRenderer* m_RendererCom = {};

protected:
	UINT m_objCBIndex = -1;

public:
	void Free() override;
};

END