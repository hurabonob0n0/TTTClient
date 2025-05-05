#pragma once
#include "RenderObject.h"
#include "VIBuffer_Geos.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Geos;
END

class CBaseGeo : public CRenderObject
{
public:
	CBaseGeo();
	virtual ~CBaseGeo() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Tick(float fTimeDelta);
	virtual void	LateTick(float fTimeDelta);
	virtual void	Render();

public:
	static CBaseGeo* Create();
	virtual void Free() override;
};

