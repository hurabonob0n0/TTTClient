#pragma once
#include "GameInstance.h"
#include "Timer.h"


using Microsoft::WRL::ComPtr;

IMPLEMENT_SINGLETON(CGameInstance)



CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(HWND windowHandle, CRawInput_Device* InputDev)
{
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#pragma region Create
	//-----------------------------Input Dev-----------------------------
	m_Input_Dev = InputDev;
	Safe_AddRef(m_Input_Dev);


	//---------------------------Graphic_Dev------------------------

	m_pGraphic_Device = CGraphic_Device::Create(windowHandle);
	if(nullptr == m_pGraphic_Device)
		return E_FAIL;

	ThrowIfFailed(m_pGraphic_Device->Get_CommandList()->Reset(m_pGraphic_Device->Get_CommandAlloc(), nullptr));

	//---------------------------ComMgr------------------------
	m_ComMgr = CComponentMgr::Get_Instance();
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	m_ComMgr->AddPrototype("TransformCom", CTransform::Create());
	m_ComMgr->AddPrototype("BaseGeoCom", CVIBuffer_Geos::Create(Get_Device(), Get_CommandList()));

	////----------------------------Renderer
	m_MainRenderer = CRenderer::Create(m_pGraphic_Device->Get_Device(), m_pGraphic_Device->Get_CommandList());
	m_ComMgr->AddPrototype("RendererCom", m_MainRenderer);
	Safe_AddRef(m_MainRenderer);

	//-------------------------Root Signature---------------------------

	m_RootSignature = CRootSignature::Create(m_pGraphic_Device->Get_Device());
	if (nullptr == m_RootSignature)
		return E_FAIL;

	
	//------------------------ShaderMgr-------------------------------------------

	m_ShaderMgr = CShader_Mgr::Get_Instance();
	if (nullptr == m_ShaderMgr)
		return E_FAIL;

	m_ShaderMgr->AddShader("VS", CShader::ST_VS, L"../Bin/ShaderFiles/color.hlsl", nullptr);
	m_ShaderMgr->AddShader("PS", CShader::ST_PS, L"../Bin/ShaderFiles/color.hlsl", nullptr);

	//---------------------PSOMgr---------------------------------------------------

	m_PSOMgr = CPSOMgr::Get_Instance();
	m_PSOMgr->AddPSO("PSO", m_RootSignature, m_ShaderMgr->GetShaderObj("VS"), m_ShaderMgr->GetShaderObj("PS"), m_pGraphic_Device, m_pGraphic_Device->Get_Device());

	//---------------------FrameResourceMgr-------------------------------------------------

	m_FrameResourceMgr = CFrameResourceMgr::Get_Instance();
	m_FrameResourceMgr->Initialize(Get_Device(), gNumFrameResources);


	//-------------------------------------ObjMgr-------------------------------------

	m_ObjMgr = CObjectMgr::Get_Instance();

	m_ObjMgr->AddObject("Camera", CCamera::Create());
	//m_ObjMgr->AddObject("BaseObj", CBoxObj::Create());


	//--------------------------------------------------------------------------------------

#pragma endregion

	ThrowIfFailed(m_pGraphic_Device->Get_CommandList()->Close());
	ID3D12CommandList* cmdsLists[] = { m_pGraphic_Device->Get_CommandList() };
	m_pGraphic_Device->Get_CommandQueue()->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	m_pGraphic_Device->FlushCommandQueue();

	return S_OK;
}

#pragma region GraphicDev

ID3D12Device* CGameInstance::Get_Device() const
{
	 return m_pGraphic_Device->Get_Device(); 
}

ID3D12GraphicsCommandList* CGameInstance::Get_CommandList() const
{
	return m_pGraphic_Device->Get_CommandList(); 
}

ID3D12CommandQueue* CGameInstance::Get_CommandQueue() const
{
	return m_pGraphic_Device->Get_CommandQueue(); 
}

ID3D12CommandAllocator* CGameInstance::Get_CommandAlloc() const
{
	return m_pGraphic_Device->Get_CommandAlloc(); 
}

ID3D12Fence* CGameInstance::Get_Fence() const
{
	return m_pGraphic_Device->Get_Fence();
}

UINT64& CGameInstance::Get_Fence_Value()
{
	return m_pGraphic_Device->Get_Fence_Value();
}

void CGameInstance::Draw_1()
{
	m_pGraphic_Device->Draw_1();
}

void CGameInstance::Draw_2()
{
	m_pGraphic_Device->Draw_2();
}

#pragma endregion

//------------------For ComMgr
HRESULT CGameInstance::AddPrototype(const string& prototypeTag, CComponent* pPrototype)
{
	if(FAILED(m_ComMgr->AddPrototype(prototypeTag, pPrototype)))
		return E_FAIL;
	return S_OK;
}

CComponent* CGameInstance::GetComponent(const string& prototypeTag, void* pArg) const
{
	return m_ComMgr->GetComponent(prototypeTag, pArg);
}

//----------------------------------For ShaderMgr---------------------------------------------

CShader* CGameInstance::GetShaderObj(const string& shaderName) const
{
	return m_ShaderMgr->GetShaderObj(shaderName);
}

BYTE* CGameInstance::GetShader(const string& shaderName) const
{
	return m_ShaderMgr->GetShader(shaderName);
}

SIZE_T CGameInstance::GetBufferSize(const string& shaderName) const
{
	return m_ShaderMgr->GetBufferSize(shaderName);
}

//-------------------------For PSOMgr----------------------------------------------------------

HRESULT CGameInstance::AddPSO(const string& PSOName, CRootSignature* pRootSignature, CShader* VS, CShader* PS, CGraphic_Device* pGD, ID3D12Device* pDevice)
{
	return m_PSOMgr->AddPSO(PSOName,pRootSignature,VS,PS,pGD,pDevice);
}

CPSO* CGameInstance::GetPSOObj(const string& PSOName) const
{
	return m_PSOMgr->GetPSOObj(PSOName);
}

ID3D12PipelineState* CGameInstance::Get_PSO(const string& PSOName) const
{
	return m_PSOMgr->Get(PSOName);
}



HRESULT CGameInstance::AddObject(const string& layerTag, CGameObject* pGameObject)
{
	return m_ObjMgr->AddObject(layerTag,pGameObject);
}

//HRESULT CGameInstance::RemoveObject(const string& layerTag, CGameObject* pGameObject)
//{
//	return E_NOTIMPL;
//}

void CGameInstance::Update(const CTimer* gameTimer)
{
	AdvanceFrame();
	auto CurrFrameResource = GetCurrentFrameResource();

	// Has the GPU finished processing the commands of the current frame resource?
	// If not, wait until the GPU has completed commands up to this fence point.
	if (CurrFrameResource->m_Fence != 0 && m_pGraphic_Device->Get_Fence()->GetCompletedValue() < CurrFrameResource->m_Fence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(m_pGraphic_Device->Get_Fence()->SetEventOnCompletion(CurrFrameResource->m_Fence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}

	const float fTimeDelta = gameTimer->DeltaTime();

	m_ObjMgr->Update(fTimeDelta);
	m_ObjMgr->LateUpdate(fTimeDelta);

}

void CGameInstance::Draw()
{
	if(!m_MainRenderer)
		return;

	auto cmdListAlloc = GetCurrentFrameResource()->m_CmdListAlloc;

	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(cmdListAlloc->Reset());

	ThrowIfFailed(m_pGraphic_Device->Get_CommandList()->Reset(cmdListAlloc, Get_PSO("PSO")));

	Draw_1();

	m_pGraphic_Device->Get_CommandList()->SetGraphicsRootSignature(m_RootSignature->Get());

	// Bind per-pass constant buffer.  We only need to do this once per-pass.
	auto passCB = GetCurrentFrameResource()->m_PassCB->Resource();
	m_pGraphic_Device->Get_CommandList()->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());

	m_MainRenderer->Render();

	m_pGraphic_Device->Draw_2();

	GetCurrentFrameResource()->m_Fence = ++m_pGraphic_Device->Get_Fence_Value();

	// Add an instruction to the command queue to set a new fence point. 
	// Because we are on the GPU timeline, the new fence point won't be 
	// set until the GPU finishes processing all the commands prior to this Signal().
	m_pGraphic_Device->Get_CommandQueue()->Signal(m_pGraphic_Device->Get_Fence(), m_pGraphic_Device->Get_Fence_Value());

	m_MainRenderer->ResetRenderObjects();
}

void CGameInstance::OnResize()
{
	if (!m_pGraphic_Device)
		return;

	m_pGraphic_Device->OnResize();
}



void CGameInstance::Free()
{
	Safe_Release(m_pGraphic_Device);
}

