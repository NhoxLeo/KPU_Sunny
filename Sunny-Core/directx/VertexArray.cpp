#include "VertexArray.h"

namespace sunny
{
	namespace directx
	{
		VertexArray::VertexArray()
		{}

		VertexArray::~VertexArray()
		{}

		void VertexArray::PushBuffer(VertexBuffer* buffer)
		{
			m_buffers.push_back(buffer);
		}

		void VertexArray::Draw(unsigned int count) const
		{
			for (unsigned int i = 0; i < m_buffers.size(); ++i)
			{
				VertexBuffer* buffer = static_cast<VertexBuffer*>(m_buffers[i]);

				buffer->Bind();

				// �������� �׸��� ������ ������ �����Ϸ��� ���� �޼��带 ȣ������ �Ѵ�.
				// void ID3D11DeviceContext::Draw(UINT VertexCount, UINT StartVertexLocation);
				// VertexCount         : ���� ����
				// StartVertexLocation : ���� ��ġ


				// �ε��� ���۸� �̿��ؼ� �⺻ ������ �׸� ������ Draw �޼��� ��� DrawIndexed �޼��带 ����ؾ� �Ѵ�.
				// void ID3D11DeviceContext::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
				// IndexCount         : �� �׸��� ȣ�⿡�� ����� �ε����� �����̴�. �̰��� �ε��� ������ ��ü ũ��(��� �ε����� ����)�� �ʿ�� ����. ��, �ε������� ���ӵ� �� �κ����Ѹ� ����ϴ� ���� �����ϴ�.
				// StartIndexLocation : ����� ù �ε����� ��ġ(���� �ȿ����� �ε���). 
				// BaseVertexLocation : �������� �������� ����, �� �׸��� ȣ�⿡�� ����� �ε����鿡 �������� ���� ��.
				Context::GetDeviceContext()->DrawIndexed(count, 0, 0);
			}
		}

	}
}
