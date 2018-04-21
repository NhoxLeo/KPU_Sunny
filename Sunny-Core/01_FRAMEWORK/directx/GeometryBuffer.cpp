#include "GeometryBuffer.h"

namespace sunny
{
	namespace directx
	{
		GeometryBuffer* GeometryBuffer::s_instance = nullptr;

		GeometryBuffer::GeometryBuffer()
		{
			m_context = Context::GetContext();
		}

		void GeometryBuffer::Init()
		{
			if (s_instance) return;

			s_instance = new GeometryBuffer();
			s_instance->InitInternal();
		}

		void GeometryBuffer::InitInternal()
		{
			D3D11_SAMPLER_DESC renderTargetSamplerDesc = {};
			{
				renderTargetSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				renderTargetSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				renderTargetSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				renderTargetSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				renderTargetSamplerDesc.MaxAnisotropy = 16;
				renderTargetSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			}
			
			D3D11_SAMPLER_DESC depthStencilSamplerDesc = {};
			{
				depthStencilSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				depthStencilSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
				depthStencilSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
			}


			directx::Context::GetDevice()->CreateSamplerState(&renderTargetSamplerDesc, &m_samplers[0]);
			directx::Context::GetDevice()->CreateSamplerState(&depthStencilSamplerDesc, &m_samplers[1]);

			Resize();
		}

		void GeometryBuffer::Resize()
		{
			m_width  = m_context->GetWindowProperties().width;
			m_height = m_context->GetWindowProperties().height;

			InitRenderTarget();
			InitDepthStencil();
		}

		void GeometryBuffer::InitRenderTarget()
		{
			// ��ǻ��/�븻 �ؽ��� ���� ����ü
			D3D11_TEXTURE2D_DESC diffuseNormalTextureDesc;
			ZeroMemory(&diffuseNormalTextureDesc, sizeof(diffuseNormalTextureDesc));
			{
				diffuseNormalTextureDesc.Width              = m_width;
				diffuseNormalTextureDesc.Height             = m_height;
				diffuseNormalTextureDesc.MipLevels          = 1;
				diffuseNormalTextureDesc.ArraySize          = 1;
				diffuseNormalTextureDesc.Format             = DXGI_FORMAT_R32G32B32A32_FLOAT; // 32bit
				diffuseNormalTextureDesc.SampleDesc.Count   = 1;
				diffuseNormalTextureDesc.SampleDesc.Quality = 0;
				diffuseNormalTextureDesc.Usage              = D3D11_USAGE_DEFAULT;
				diffuseNormalTextureDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				diffuseNormalTextureDesc.CPUAccessFlags     = 0;
				diffuseNormalTextureDesc.MiscFlags          = 0;
			}

			// �ؽ��� ����
			ID3D11Texture2D* diffuseNormalTextures[2];
			m_context->GetDevice()->CreateTexture2D(&diffuseNormalTextureDesc, NULL, &diffuseNormalTextures[GeometryTextureType::DIFFUSE]);
			m_context->GetDevice()->CreateTexture2D(&diffuseNormalTextureDesc, NULL, &diffuseNormalTextures[GeometryTextureType::NORMAL]);


			// ��ǻ��/�븻 ���� Ÿ�� �� ���� ����ü
			D3D11_RENDER_TARGET_VIEW_DESC diffuseNormalRenderTargetViewDesc;
			ZeroMemory(&diffuseNormalRenderTargetViewDesc, sizeof(diffuseNormalRenderTargetViewDesc));
			{
				diffuseNormalRenderTargetViewDesc.Format             = diffuseNormalTextureDesc.Format;
				diffuseNormalRenderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
				diffuseNormalRenderTargetViewDesc.Texture2D.MipSlice = 0;
			}
			
			// ���� Ÿ�� �� ����
			m_context->GetDevice()->CreateRenderTargetView(diffuseNormalTextures[GeometryTextureType::DIFFUSE], &diffuseNormalRenderTargetViewDesc, &m_renderTargetViews[GeometryTextureType::DIFFUSE]);
			m_context->GetDevice()->CreateRenderTargetView(diffuseNormalTextures[GeometryTextureType::NORMAL],  &diffuseNormalRenderTargetViewDesc, &m_renderTargetViews[GeometryTextureType::NORMAL]);


			// ��ǻ��/�븻 ���̴� ���ҽ� �� ���� ����ü
			D3D11_SHADER_RESOURCE_VIEW_DESC diffuseNormalShaderResourceViewDesc;
			ZeroMemory(&diffuseNormalShaderResourceViewDesc, sizeof(diffuseNormalShaderResourceViewDesc));
			{
				diffuseNormalShaderResourceViewDesc.Format                    = diffuseNormalTextureDesc.Format;
				diffuseNormalShaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
				diffuseNormalShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
				diffuseNormalShaderResourceViewDesc.Texture2D.MipLevels       = 1;
			}
			
			// ���̴� ���ҽ� �� ����
			m_context->GetDevice()->CreateShaderResourceView(diffuseNormalTextures[GeometryTextureType::DIFFUSE], &diffuseNormalShaderResourceViewDesc, &m_shaderResourceViews[GeometryTextureType::DIFFUSE]);
			m_context->GetDevice()->CreateShaderResourceView(diffuseNormalTextures[GeometryTextureType::NORMAL],  &diffuseNormalShaderResourceViewDesc, &m_shaderResourceViews[GeometryTextureType::NORMAL]);
		

			// �ؽ�ó ������
			diffuseNormalTextures[GeometryTextureType::DIFFUSE]->Release();
			diffuseNormalTextures[GeometryTextureType::NORMAL]->Release();
		}

		void GeometryBuffer::InitDepthStencil()
		{
			// ����/���ٽ� �ؽ��� ���� ����ü
			D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
			ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
			{
				depthStencilTextureDesc.Width              = m_width;
				depthStencilTextureDesc.Height             = m_height;
				depthStencilTextureDesc.MipLevels          = 1;
				depthStencilTextureDesc.ArraySize          = 1;
				depthStencilTextureDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS;
				depthStencilTextureDesc.SampleDesc.Count   = 1;
				depthStencilTextureDesc.SampleDesc.Quality = 0;
				depthStencilTextureDesc.Usage              = D3D11_USAGE_DEFAULT;
				depthStencilTextureDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
				depthStencilTextureDesc.CPUAccessFlags     = 0;
				depthStencilTextureDesc.MiscFlags          = 0;
			}

			// ����/���ٽ� �ؽ��� ����
			ID3D11Texture2D* depthStencilTexture = nullptr;
			m_context->GetDevice()->CreateTexture2D(&depthStencilTextureDesc, NULL, &depthStencilTexture);


			// ����/���ٽ� �� ���� ����ü
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
			{
				depthStencilViewDesc.Flags              = 0;
				depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;
			}

			// ����/���ٽ� �� ����
			m_context->GetDevice()->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView);


			// ����/���ٽ� ���̴� ���ҽ� �� ���� ����ü
			D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilShaderResourceViewDesc;
			ZeroMemory(&depthStencilShaderResourceViewDesc, sizeof(depthStencilShaderResourceViewDesc));
			{
				depthStencilShaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				depthStencilShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				depthStencilShaderResourceViewDesc.Texture2D.MipLevels = 1;
				depthStencilShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			}

			// ���̴� ���ҽ� �� ����
			m_context->GetDevice()->CreateShaderResourceView(depthStencilTexture, &depthStencilShaderResourceViewDesc, &m_shaderResourceViews[GeometryTextureType::DEPTH]);


			// �ؽ�ó ���� ����
			depthStencilTexture->Release();
		}

		void GeometryBuffer::BindInternal()
		{
			//m_context->GetDeviceContext()->RSSetViewports(1, &m_viewport);
			m_context->GetDeviceContext()->OMSetRenderTargets(2, m_renderTargetViews, m_depthStencilView);
		}

		void GeometryBuffer::UnBindInternal()
		{
			m_context->Bind();
		}
	}
}