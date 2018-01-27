#include "IndexBuffer.h"

namespace sunny
{
	namespace directx
	{
		IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count) : m_count(count)
		{
			// �ε��� ���۸� �����ϴ� ����ü�� ä���. (��ü������ ���� ���ۿ� ����ϴ�.)
			D3D11_BUFFER_DESC ibd;
			ZeroMemory(&ibd, sizeof(D3D11_BUFFER_DESC));

			ibd.Usage = D3D11_USAGE_IMMUTABLE;                               // D3D11_USAGE_IMMUTABLE : �Һ��� �ǹ��Ѵ�.
			ibd.ByteWidth = count * sizeof(unsigned int);                      
			ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;                         // �ε��� ������ ��쿡�� D3D11_BIND_INDEX_BUFFER�� �����Ѵ�.
			ibd.CPUAccessFlags = 0;
			ibd.MiscFlags = 0;

			// �ε��� ���۸� �ʱ�ȭ�� �ڷḦ �����Ѵ�.
			D3D11_SUBRESOURCE_DATA ibInitData;
			ibInitData.pSysMem = data;                                       // pSysMem: ���۸� �ʱ�ȭ�� �ڷḦ ���� �ý��� �޸� �迭�� ����Ű�� ������


			HRESULT hr = Context::GetDevice()->CreateBuffer(&ibd, &ibInitData, &m_bufferHandle);
		}

		void IndexBuffer::Bind() const
		{
			// �Է� ������ �ܰ迡 ������Ƽ�� Ÿ���� �����Ѵ�.
			Context::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			// D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : ���� ������ �ﰢ�� ������� ���Ѵ�.


			// �ε��� ���۸� ����Ϸ��� ���� �ε��� ���۸� ������ ���ο� ����� �Ѵ�. �ε��� ���۴� �Է� ������ �ܰ迡 ����� �Ѵ�.
			Context::GetDeviceContext()->IASetIndexBuffer(m_bufferHandle, DXGI_FORMAT_R32_UINT, 0);
			// DXGI_FORMAT_R32_UINT: 32��Ʈ ��ȣ ���� ������ ���Ѵ�.
			// �� D3D11_BUFFER_DESC::ByteWidth ����� ������ �����Ƿ�, �� ���� ������� �ʵ��� �ϴ� ���� �߿��ϴ�.
		}

	}
}