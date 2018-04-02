#include "DeferredBuffer.h"

namespace sunny
{
	namespace directx
	{
		DeferredBuffer* DeferredBuffer::s_instance = nullptr;

		DeferredBuffer::DeferredBuffer()
		{
			m_context = Context::GetContext();
		}

		void DeferredBuffer::Init()
		{
			s_instance = new DeferredBuffer();
			s_instance->InitInternal();
		}

		void DeferredBuffer::InitInternal()
		{
			Resize();
		}

		void DeferredBuffer::Resize()
		{
			int width  = Context::GetWindowProperties().width;
			int height = Context::GetWindowProperties().height;

			// �ؽ��� ���� ����ü
			D3D11_TEXTURE2D_DESC textureDescPosNorm; // ������, �븻 (32bit)
			D3D11_TEXTURE2D_DESC textureDescDiffuse; // ���̽� ����  ( 8bit)

			// ������, �븻
			{
				textureDescPosNorm.Width = width;
				textureDescPosNorm.Height = height;
				textureDescPosNorm.MipLevels = 1;
				textureDescPosNorm.ArraySize = 1;
				textureDescPosNorm.Format =  DXGI_FORMAT_R32G32B32A32_FLOAT; // 32bit
				textureDescPosNorm.SampleDesc.Count = 1;
				textureDescPosNorm.SampleDesc.Quality = 0;
				textureDescPosNorm.Usage = D3D11_USAGE_DEFAULT;
				textureDescPosNorm.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDescPosNorm.CPUAccessFlags = 0;
				textureDescPosNorm.MiscFlags = 0;
			}

			// ���̽� ����
			{
				textureDescDiffuse.Width = width;
				textureDescDiffuse.Height = height;
				textureDescDiffuse.MipLevels = 1;
				textureDescDiffuse.ArraySize = 1;
				textureDescDiffuse.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  // 8bit
				textureDescDiffuse.SampleDesc.Count = 1;
				textureDescDiffuse.SampleDesc.Quality = 0;
				textureDescDiffuse.Usage = D3D11_USAGE_DEFAULT;
				textureDescDiffuse.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDescDiffuse.CPUAccessFlags = 0;
				textureDescDiffuse.MiscFlags = 0;
			}
			
		
			// �ؽ��� ����
			m_textures[0] = nullptr;
			m_context->GetDevice()->CreateTexture2D(&textureDescPosNorm, NULL, &m_textures[0]);
			m_context->GetDevice()->CreateTexture2D(&textureDescPosNorm, NULL, &m_textures[1]);
			m_context->GetDevice()->CreateTexture2D(&textureDescPosNorm, NULL, &m_textures[2]);

			// ���� Ÿ�� �� ���� ����ü (32bit)
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc32;
			ZeroMemory(&renderTargetViewDesc32, sizeof(renderTargetViewDesc32));
			{
				renderTargetViewDesc32.Format = textureDescPosNorm.Format;
				renderTargetViewDesc32.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc32.Texture2D.MipSlice = 0;
			}
	
			// ���� Ÿ�� �� ���� ����ü ( 8bit)
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc8;
			ZeroMemory(&renderTargetViewDesc8, sizeof(renderTargetViewDesc8));
			{
				renderTargetViewDesc8.Format = textureDescPosNorm.Format;
				renderTargetViewDesc8.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc8.Texture2D.MipSlice = 0;
			}

			// ���� Ÿ�� �� ����
			// pResource: ������ ���� ����ü �� �ؽ�ó�� ��������
			// pDesc    : ������ ���� ���� ���� ����ü   
			// ppRTView : ������ �並 ��ȯ�޴� ������	
			m_renderTargetViews[0] = nullptr;
			m_context->GetDevice()->CreateRenderTargetView(m_textures[0], &renderTargetViewDesc32, &m_renderTargetViews[0]);
			m_context->GetDevice()->CreateRenderTargetView(m_textures[1], &renderTargetViewDesc32, &m_renderTargetViews[1]);
			m_context->GetDevice()->CreateRenderTargetView(m_textures[2], &renderTargetViewDesc8 , &m_renderTargetViews[2]);

			// ���̴� ���ҽ� �� ���� ����ü
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescDR;
			ZeroMemory(&shaderResourceViewDescDR, sizeof(shaderResourceViewDescDR));
			shaderResourceViewDescDR.Format = textureDescPosNorm.Format;
			shaderResourceViewDescDR.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDescDR.Texture2D.MostDetailedMip = 0;
			shaderResourceViewDescDR.Texture2D.MipLevels = 1;

			// ���̴� ���ҽ� �� ����
			for (int i = 0; i < BUFFER_COUNT; ++i)
				m_context->GetDevice()->CreateShaderResourceView(m_textures[i], &shaderResourceViewDescDR, &m_shaderResourceViews[i]);

			// Release texture
			for (int i = 0; i < BUFFER_COUNT; ++i)
				m_textures[i]->Release();

			// ���� / ���� �ؽ��� ���� ����ü
			D3D11_TEXTURE2D_DESC depthBufferDescDR;
			ZeroMemory(&depthBufferDescDR, sizeof(depthBufferDescDR));
			{
				depthBufferDescDR.Width = width;
				depthBufferDescDR.Height = height;
				depthBufferDescDR.MipLevels = 1;
				depthBufferDescDR.ArraySize = 1;
				//depthBufferDescDR.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthBufferDescDR.Format = DXGI_FORMAT_R24G8_TYPELESS;
				depthBufferDescDR.SampleDesc.Count = 1;
				depthBufferDescDR.SampleDesc.Quality = 0;
				depthBufferDescDR.Usage = D3D11_USAGE_DEFAULT;
				depthBufferDescDR.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
				depthBufferDescDR.CPUAccessFlags = 0;
				depthBufferDescDR.MiscFlags = 0;
			}
			m_context->GetDevice()->CreateTexture2D(&depthBufferDescDR, 0, &m_depthStencilBuffer);

			// ���� / ���ٽ� ���� Ÿ�ٻ� ��
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescDR;
			ZeroMemory(&depthStencilViewDescDR, sizeof(depthStencilViewDescDR));
			{
				depthStencilViewDescDR.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilViewDescDR.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDescDR.Texture2D.MipSlice = 0;
			}
			//Create depth stencil view
			m_context->GetDevice()->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDescDR, &m_depthStencilView);
			// pResource: ��� ���� DepthStecil ����
			// pDesc    : ��� ���� DepthStecil ���� ����ü (NULL�� �⺻ ����)
			// ppRTView : ������ DepthStencil�� ��ȯ�޴� ������
		}

		void DeferredBuffer::BindInternal()
		{
			m_context->GetDeviceContext()->OMSetRenderTargets(3, m_renderTargetViews, m_depthStencilView);
		}

		void DeferredBuffer::UnBindInternal()
		{
			m_context->Bind();
		}

	}
}