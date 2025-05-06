#pragma once
#include "Base.h"
#include "GameObject.h"
#include "RenderObject.h"
#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CObjectMgr : public CBase
{
    DECLARE_SINGLETON(CObjectMgr)

private:
    CObjectMgr();
    virtual ~CObjectMgr() = default;

public:
    typedef vector<CGameObject*> Layer;

    HRESULT AddObject(const string& layerTag, CGameObject* pGameObject);
    HRESULT RemoveObject(const string& layerTag, CGameObject* pGameObject);


    void Update(const float& fTimeDelta);
    void LateUpdate(const float& fTimeDelta);

private:
    unordered_map<string, Layer> m_Layers;

public:
    void Free() override;
};

END