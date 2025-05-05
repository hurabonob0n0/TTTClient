#pragma once
#include "Base.h"
#include "Graphic_Device.h"
#include "ComponentMgr.h"
#include "Shader_Mgr.h"
#include "RootSignatures.h"
#include "PSOMgr.h"
#include "FrameResourceMgr.h"
#include "ObjectMgr.h"
#include "Raw_Input.h"

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	void OnResize();

public:
	HRESULT Initialize_Engine(HWND windowHandle, CRawInput_Device* InputDev);

public:			// For Graphic_Dev
	ID3D12Device* Get_Device() const; 
	ID3D12GraphicsCommandList* Get_CommandList() const; 
	ID3D12CommandQueue* Get_CommandQueue() const; 
	ID3D12CommandAllocator* Get_CommandAlloc() const; 
	ID3D12Fence* Get_Fence() const; 
	UINT64& Get_Fence_Value();
	void Draw_1();
	void Draw_2();


public:			// For ComMgr
	HRESULT					AddPrototype(const string& prototypeTag, CComponent* pPrototype);
	CComponent*				GetComponent(const string& prototypeTag, void* pArg = nullptr) const;


public:			// For RootSignature
	ID3D12RootSignature*	Get_RootSignature() const { return m_RootSignature->Get(); }


public:			// For ShaderMgr
	CShader*				GetShaderObj(const string& shaderName) const;
	BYTE*					GetShader(const string& shaderName) const;
	SIZE_T					GetBufferSize(const string& shaderName) const;

public:			// For PSOMgr
	HRESULT					AddPSO(const string& PSOName, CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice);
	CPSO*					GetPSOObj(const string& PSOName) const;
	ID3D12PipelineState*	Get_PSO(const string& PSOName) const;

public:			// For FrameResourceMgr
	CFrameResource*			GetCurrentFrameResource() const { return m_FrameResourceMgr->GetCurrentFrameResource(); };
	UINT					GetCurrentFrameIndex() const { return m_FrameResourceMgr->GetCurrentFrameIndex(); }
	void					AdvanceFrame() { m_FrameResourceMgr->AdvanceFrame(); };

public:			// For ObjMgr
	HRESULT AddObject(const string& layerTag, CGameObject* pGameObject);
	//HRESULT RemoveObject(const string& layerTag, CGameObject* pGameObject);

public:
	void Update(const class CTimer* gameTimer);
	void Draw();

public:
	class CGraphic_Device*			m_pGraphic_Device;

private:
	CComponentMgr*					m_ComMgr;
	CRootSignature*					m_RootSignature;
	CShader_Mgr*					m_ShaderMgr;
	CPSOMgr*						m_PSOMgr;
	CFrameResourceMgr*				m_FrameResourceMgr;
	CRenderer*						m_MainRenderer;
	CObjectMgr*						m_ObjMgr;
	CRawInput_Device*				m_Input_Dev;

private:
	vector<class CGameObject*>		m_vGameObjects;
	

public:
	virtual void Free() override;
};

END