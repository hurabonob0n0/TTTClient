#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	CVIBuffer(ID3D12Device* pDevice);
	CVIBuffer(CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;



protected:
	UINT m_VertexByteStride = 0;
	UINT m_VertexBufferByteSize = 0;
protected:
	DXGI_FORMAT	m_IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT		m_IndexBufferByteSize = 0;
protected:
	D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

public:
	virtual HRESULT Initialize_Prototype() = 0;
	virtual HRESULT Initialize(void* pArg) = 0;

public:
	CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END