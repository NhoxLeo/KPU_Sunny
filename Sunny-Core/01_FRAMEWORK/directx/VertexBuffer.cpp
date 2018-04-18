#include "VertexBuffer.h"

namespace sunny
{
	namespace directx
	{
		VertexBuffer::VertexBuffer() : m_size(0)
		{
			ZeroMemory(&m_bufferDesc, sizeof(D3D11_BUFFER_DESC));

			                                                                                                               // ByteWidth : ������ ���� ������ ũ��(����Ʈ ����)
			m_bufferDesc.Usage          = D3D11_USAGE_DYNAMIC;															   // Usage     : ���۰� ���̴� ��� (D3D11_USAGE_DEFAULT  vs ..)
			m_bufferDesc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;                                                        // BindFlags : ���� ������ ��쿡�� D3D11_BIND_VERTEX_BUFFER�� �����Ѵ�.
			m_bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                                                          // CPUAccessFlags : CPU�� ���ۿ� �����ϴ� ����� �����ϴ� �÷��� (CPU�� �ڷḦ ����ؼ� ���۸� �����ؾ� �Ѵٸ� D3D11_CPU_ACCESS_WRITE)
			                                                                                                               // MiscFlags : ���� ���ۿ� ���ؼ��� �� ��Ÿ �÷��׵��� ����� �ʿ䰡 ����. (�׳� 0)
																														   // StructureByteStride: ������ ����(structured buffer)�� ���Ƶ� ���� �ϳ��� ũ��(����Ʈ ����), ������ ���۰� �ƴҰ��� 0
		}  

		VertexBuffer::~VertexBuffer()
		{
		}

		void VertexBuffer::Resize(unsigned int size)
		{
			m_size = size;
			m_bufferDesc.ByteWidth = size;                                                                                 // ByteWidth : ������ ���� ������ ũ��(����Ʈ ����)

			// ���۸� �����ϱ� ���ؼ� ID3D11Device::CreateBuffer() �Լ��� ����Ѵ�.
			Context::GetContext()->GetDevice()->CreateBuffer(&m_bufferDesc, NULL, &m_bufferHandle);
			// pDesc        : ������ ���۸� �����ϴ� D3D11_BUFFER_DESC ����ü�� ������
			// pInitialData : ������ ���۸� �ʱ�ȭ �ϱ� ���� ������(D3D11_SUBRESOURCE_DATA)�� ������, �� ���� NULL�̸� �޸𸮸� �Ҵ��ϰ� �ʱ�ȭ���� �ʴ´�. 
			// ppBuffer     : ������ ���� �������̽��� ��ȯ�� �������� �ּ�
		}

		void VertexBuffer::SetData(unsigned int size, const void* data)
		{
			if (m_size < size) Resize(size);

			GetPointerInternal();
			memcpy(m_mappedSubresource.pData, data, size);
			ReleasePointer();
		}

		void VertexBuffer::SetLayout(const BufferLayout& bufferLayout)
		{
			m_layout = bufferLayout;
			const std::vector<BufferElement>& layout = m_layout.GetLayout();

			// ���� ����ü�� �� ����(�ʵ�)�� � �뵵������ Direct3D���� �˷� �־�� �Ѵ�. 
			// �׷��� ������ Direct3D���� �˷� �ִ� ������ �ٷ� ID3D11InputLayout ������ �Է� ��ġ(input layout) ��ü��. 
			// �Է� ��ġ�� D3D11_INPUT_ELEMENT_DESC ����ü��� �̷���� �迭�� ���ؼ� �����Ѵ�.
			D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC[layout.size()];

			for (unsigned int i = 0; i < layout.size(); i++)
			{
				const BufferElement& element = layout[i];
				desc[i] = { element.name.c_str(), 0, (DXGI_FORMAT)element.type, 0, element.offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				// SemanticName         : ���п� �ο��� ���ڿ� �̸�. �̰��� ���� ���̴����� sematic �̸����� ���̹Ƿ� �ݵ�� ��ȿ�� ���� �̸��̾�� �Ѵ�.
				// SematicIndex         : semantic�� �ο��� �ε���. (�ϳ��� ���� ����ü�� �ؽ��� ��ǥ�� ���� �� �ִ� ��� �� �ؽ�ó ��ǥ���� �������� semantic�� �ο��ϴ� ��� ��ó�� �ε����� ���ذ� ������ �� �ִ�.) ���̴� �ڵ忡�� �ε����� �������� ���� semantic�� 0���� ���ֵȴ�.
				// Fortmat              : DXGI_FORMAT �������� �� �����, �� ���� ������ �ڷ� ������ ��Ÿ����.
				// InputSlot            : �ڷᰡ ���޵� ���� ���� ������ �ε�����. Direct3D�� 16���� ���� ���� ���Ե��� �����Ѵ�.
				// AligendByteOffset    : �Է� ������ �ϳ��� ����ϴ� ��� �� �ʵ�� C++ ���� ����ü�� ���� ��ġ�� �� ���� ������ ��ġ ������ ������(����Ʈ ����)�̴�.
				// InputSlotClass       : ���� D3D11_INPUT_PER_VERTEX_DATA ���� ������.
				// InstanceDataStepRate : ���� 0�� ���� ������.
			}

			const Shader* shader = Shader::CurrentlyBound();

			// �Է� ��ġ ���� ����ü �迭�� �ϼ��ϰ� �Է� ��ġ�� ���� ID3D11InputLayout �������̽��� ����ٸ�, 
			// ���� ID3D11Device::CreateInputLayout �޼��带 ȣ���ؼ� �Է� ��ġ�� �����Ѵ�.
			Context::GetDevice()->CreateInputLayout(desc, layout.size(), shader->GetData().vs->GetBufferPointer(), shader->GetData().vs->GetBufferSize(), &m_inputLayout);
			// pInputElementDescs                : ���� ����ü�� �����ϴ� D3D11_INPUT_ELEMENT_DESC ���� �迭.
			// NumElements                       : �� D3D11_INPUT_ELEMENT_DESC �迭�� ���� ����.
			// pShaderBytecodeWithInputSignature : ���� ���̴�(�Է� ������ ������)�� �������ؼ� ���� ����Ʈ �ڵ带 ����Ű�� ������.
			// BytecodeLength                    : �� ����Ʈ �ڵ��� ũ��(����Ʈ ����).
			// ppInputLayout                     : ������ �Է� ��ġ�� �� �����͸� ���ؼ� �����ش�.
			delete desc;
		}

		void* VertexBuffer::GetPointerInternal()
		{
			// ���� ���ҽ��� ���Ե� �����Ϳ� ���� �����͸� ���������� ID3D11DeviceContext::Map() �Լ��� ����Ѵ�. �� �Լ��� ȣ���ϸ� GPU�� ���� ���ҽ��� ������ �� ���� �ȴ�.
			// ��, ���� ���ۿ� ���� �޸� �ּҸ� �����´�.
			Context::GetDeviceContext()->Map(m_bufferHandle, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &m_mappedSubresource);
			// pResource       : ID3D11Resource �������̽��� ���� �����ʹ�.
			// SubResource     : ���� ���ҽ��� �ε���
			// MapType         : ���ҽ��� ���� CPU�� �б�/���� ������ ��Ÿ���� D3D11_MAP ������ �����.        
			//    ��             D3D11_MAP_WRITE_DISCARD �� ���ҽ��� ������ �����Ѵ�. ���ҽ��� ���� ������ �������� �ʴ´�.
			// MapFlags        : GPU�� ���ν��� �����ϰ� ���� �� CPU�� ��� �� ���ΰ��� ��Ÿ���� �����.
			// pMappedResource : ���ε� ���� ���ҽ��� ���� �����ʹ�.

			return m_mappedSubresource.pData;
		}

		void VertexBuffer::ReleasePointer()
		{
			Context::GetDeviceContext()->Unmap(m_bufferHandle, NULL);
		}

		void VertexBuffer::Bind()
		{
			// ���� ���۸� ����Ϸ��� �Է� ���̾ƿ� �������̽� ��ü�� �����Ͽ� ���� ������ ���̾ƿ��� �����ؾ� �Ѵ�.
			unsigned int offset = 0;
			unsigned int stride = m_layout.GetStride();
			
			// �̷��� ������ �Է� ��ġ�� ���� ��ġ�� ������ ���� �����̴�.
			// ���� �Է� ��ġ ������ ������ �ܰ�� �Է� ��ġ�� ����ϰ��� �ϴ� ��ġ�� ���� ���̴�.
			Context::GetDeviceContext()->IASetInputLayout(m_inputLayout);

			// ID3D11DeviceContext::IASetVertexBuffers() �Լ��� �̿��� ���� ���۸� �Է� �����ܰ迡 ���� �Ѵ�.
			Context::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_bufferHandle, &stride, &offset);
			// StartSlot       : ������ ù ��° �Է� ���� �ε�����. ù ��° ���� ���۴� ���� ���Կ� ����ǰ� ������� ���� ���Կ� ����ȴ�. �ִ� 16���� �Է� ������ �����Ѵ�.
			// NumBuffers      : ppVertexBuffers�� ��Ÿ���� �迭�� ���� ������. ������ ������ �Է� ���� ��ü ������ ���� �� ����.
			// ppVertexBuffers : ���� ����(ID3D11Buffer) �迭�� ���� �����ʹ�. ���� ���۴� D3D11_BIND_VERTEX_BUFFER �÷��׷� �����Ǿ�� �Ѵ�.
			// pStrides        : ���� ���� �迭�� ������ ũ��(����Ʈ)���� �迭�� ���� �����ʹ�. �� �迭�� ���� ������ ����(NumBuffers)��ŭ�� ���Ҹ� ���´�.
			// pOffsets        : ������ ������ �迭�� ���� �����ʹ�. �������� ���� ���ۿ��� ������ ���Ǵ� ù��° ���ҿ� ������ ù ��° ���һ����� ����Ʈ ����. �� �迭�� ���� ������ ����(NumBuffers)��ŭ�� ���Ҹ� ���´�.
		}

	}
}