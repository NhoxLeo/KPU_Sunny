#pragma once

#include "../include.h"
#include "Common.h"
#include "Context.h"
#include "BufferLayout.h"
#include "Shader.h"
#include "../01_FRAMEWORK/app/Application.h"

/*
GPU�� ���� �迭�� ������ �� �������� �迭�� �������� ����(buffer)��� �θ��� Ư���� �ڿ��� ��� �ξ�� �Ѵ�.
Direct3D �ڵ忡�� ���۴� ID3D11Buffer �������̽��� ��ǥ�ȴ�.

�������� ��� ���۸� ���� ���۶�� �θ���. Direct3D�� ���۵��� �ڷḦ ���� �Ӹ��ƴ϶�, CPU�� GPU�� �ڷῡ ��� ������ �� �ְ�
���۰� ������������ ��� ���̴����� ���� ������ ������. ���� ���۸� �����Ϸ��� ������ ���� ������ ��ģ��.

1. ������ ���۸� �����ϴ� D3D11_BUFFER_DESC ����ü�� �����.
2. ������ �ʱ�ȭ�� ����� �ڷḦ �����ϴ� D3D11_SUBRESOURCE_DATA ����ü�� ä���.
3. ID3D11Device::CreateBuffer�� ȣ���ؼ� ���۸� �����Ѵ�.
*/


// ���� ���۸� ����ϴ� ������ �����ϸ� ������ ����.
// 1. ���� ���۸� �����Ѵ�.
// 2. ���� ���۸� �Է� ���� �ܰ迡 �����Ѵ�.
// 3. �Է� ���̾ƿ� ��ü�� �����Ѵ�.
// 4. �Է� ���̾ƿ� ��ü�� �Է� �����ܰ迡 �����Ѵ�. 

/* �Է������ܰ� (IA) */
/*
������ ������������ ���� ó�� �ܰ��̴�. 

���ø����̼����κ��� �Ѱܹ��� �������۳� �ε��� ������ �����͸� �о� �鿩 [��], [��], [�ﰢ��]���� ������Ƽ�긦
�����Ͽ� ������������ ���� �ܰ�� �����͸� ��������� ������ �Ѵ�.

�� �ۿ� ���̴������� ó���� �ʿ��� [�ý��� ������]�� �߰��� �� �ִ�.
[�ý��� ������]�� �ٸ� ����� ��ҿ� ���������� [�ø�ƽ��]�� �ٿ� �ĺ��ϸ�
IA�� �����ϴ� �ý��� ���������� [���� ID] [������Ƽ�� ID] [�ν��Ͻ� ID]���� �ִ�.

IA���� �ٷ�� �Է� ���ۿ��� [���� ����] �� [�ε��� ����] 2 ������ ������ �� ���� ���� ���ҽ��� �� �����̴�.
*/

namespace sunny
{
	namespace directx
	{
		
		class VertexBuffer
		{
		private:
			D3D11_BUFFER_DESC m_bufferDesc;                  // ���۸� �����ϴ� ����ü
			
			D3D11_MAPPED_SUBRESOURCE m_mappedSubresource;    // ���� ���ҽ� �����Ϳ� �����ϱ� ���� ����ü
			
			ID3D11Buffer* m_bufferHandle;                    // ���� �����͸� �����ϱ� ���� ���� ���� �������ν� ������
			
			ID3D11InputLayout* m_inputLayout;                // �Է� ���̾ƿ� �������̽� ������
			
			unsigned int m_size;                             // ���� ũ��

			BufferLayout m_layout;

			DIMENSION m_dimension;

		protected:
			void* GetPointerInternal();

		public:
			VertexBuffer(DIMENSION dimension = DIMENSION::D3);
			~VertexBuffer();

			void Resize(unsigned int size);
			
			void SetLayout(const BufferLayout& layout);
			
			void SetData(unsigned int size, const void* data);

			void ReleasePointer();

			void Bind();

			template<typename T>
			T* GetPointer()
			{
				return (T*)GetPointerInternal();
			}
		};
	}
}