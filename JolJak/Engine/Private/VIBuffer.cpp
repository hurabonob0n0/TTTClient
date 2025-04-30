#include "VIBuffer.h"


CVIBuffer::CVIBuffer(ID3D12Device* pDevice, ID3D12GraphicsCommandList* pCommandList) : CComponent(pDevice,pCommandList)
{
}

CVIBuffer::CVIBuffer(CVIBuffer& rhs) : CComponent(rhs)
{
	m_VertexByteStride		= rhs.m_VertexByteStride;
	m_VertexNum				= rhs.m_VertexNum;
	m_VertexBufferByteSize	= rhs.m_VertexBufferByteSize;
							  
	m_IndexFormat			= rhs.m_IndexFormat;
	m_IndexNum				= rhs.m_IndexNum;
	m_IndexBufferByteSize	= rhs.m_IndexBufferByteSize;
							
	m_PrimitiveType			= rhs.m_PrimitiveType;
	
	m_VertexBufferGPU		= rhs.m_VertexBufferGPU;
	m_IndexBufferGPU		= rhs.m_IndexBufferGPU;
	Safe_AddRef(m_VertexBufferGPU);
	Safe_AddRef(m_IndexBufferGPU);
}

D3D12_VERTEX_BUFFER_VIEW CVIBuffer::VertexBufferView() const
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = m_VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = m_VertexByteStride;
	vbv.SizeInBytes = m_VertexBufferByteSize;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW CVIBuffer::IndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = m_IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = m_IndexFormat;
	ibv.SizeInBytes = m_IndexBufferByteSize;

	return ibv;
}

void CVIBuffer::DisposeUploaders()
{
	Safe_Release(m_VertexBufferUploader);
	Safe_Release(m_IndexBufferUploader);
}

void CVIBuffer::Free()
{
	Safe_Release(m_VertexBufferGPU);
	Safe_Release(m_IndexBufferGPU);
	Safe_Release(m_VertexBufferUploader);
	Safe_Release(m_IndexBufferUploader);
	__super::Free();
}

