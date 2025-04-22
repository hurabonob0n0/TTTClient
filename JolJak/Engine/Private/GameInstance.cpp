#pragma once
#include "GameInstance.h"
#include "Graphic_Device.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(HWND windowHandle)
{
	m_pGraphic_Device = CGraphic_Device::Create(windowHandle);
	if(nullptr == m_pGraphic_Device)
		return E_FAIL;
	return S_OK;
}

void CGameInstance::Draw()
{
	if(!m_pGraphic_Device)
		return;

	m_pGraphic_Device->Draw();
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
