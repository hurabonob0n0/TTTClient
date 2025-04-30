#include "VIBuffer.h"


CVIBuffer::CVIBuffer(ID3D12Device* pDevice) : CComponent(pDevice)
{
}

CVIBuffer::CVIBuffer(CVIBuffer& rhs) : CComponent(rhs)
{

}

CComponent* CVIBuffer::Clone(void* pArg)
{
    return nullptr;
}

void CVIBuffer::Free()
{

}
