#include "ShadowBuffer.h"

namespace sunny
{
	namespace directx
	{
		ShadowBuffer* ShadowBuffer::s_instance = nullptr;

		ShadowBuffer::ShadowBuffer()
		{
			m_context = Context::GetContext();
		}

		void ShadowBuffer::Init()
		{
			s_instance = new ShadowBuffer();
			
			s_instance->Resize();
		}

		void ShadowBuffer::Resize()
		{
			// �׸��� ���÷� ���� ����ü
			D3D11_SAMPLER_DESC shadowSamplerDesc;
			ZeroMemory(&shadowSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
			{
				shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
				shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
				shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				shadowSamplerDesc.BorderColor[0] = 1.0f;
				shadowSamplerDesc.BorderColor[1] = 1.0f;
				shadowSamplerDesc.BorderColor[2] = 1.0f;
				shadowSamplerDesc.BorderColor[3] = 1.0f;
				shadowSamplerDesc.MinLOD = 0.f;
				shadowSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
				shadowSamplerDesc.MipLODBias = 0.f;
				shadowSamplerDesc.MaxAnisotropy = 0;
				shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
				shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
			}

			// �׸��� ���÷� ����
			directx::Context::GetDevice()->CreateSamplerState(&shadowSamplerDesc, &m_sampler);


			int width  = 2048; // m_context->GetWindowProperties().width;   // 2048
			int height = 2048; // m_context->GetWindowProperties().height;  // 2048

			// �׸��� ����/���ٽ� �ؽ��� ���� ����ü
			D3D11_TEXTURE2D_DESC shadowDepthTextureDesc;
			ZeroMemory(&shadowDepthTextureDesc, sizeof(shadowDepthTextureDesc));
			{
				shadowDepthTextureDesc.Width              = width;
				shadowDepthTextureDesc.Height             = height;
				shadowDepthTextureDesc.MipLevels          = 1;
				shadowDepthTextureDesc.ArraySize          = 1;
				shadowDepthTextureDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS;
				shadowDepthTextureDesc.SampleDesc.Count   = 1;
				shadowDepthTextureDesc.SampleDesc.Quality = 0;
				shadowDepthTextureDesc.Usage              = D3D11_USAGE_DEFAULT;
				shadowDepthTextureDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
				shadowDepthTextureDesc.CPUAccessFlags     = 0;
				shadowDepthTextureDesc.MiscFlags          = 0;
			}

			// �׸��� ����/���ٽ� �ؽ��� ����
			ID3D11Texture2D* shadowDepthTexture = nullptr;
			auto d = m_context->GetDevice()->CreateTexture2D(&shadowDepthTextureDesc, NULL, &shadowDepthTexture);
			std::cout << "d: " << d << std::endl;

			// ��

			// �׸��� ����/���ٽ� �� ���� ����ü
			D3D11_DEPTH_STENCIL_VIEW_DESC shadowDepthStencilViewDesc;
			ZeroMemory(&shadowDepthStencilViewDesc, sizeof(shadowDepthStencilViewDesc));
			{
				shadowDepthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
				shadowDepthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
				shadowDepthStencilViewDesc.Texture2D.MipSlice = 0;
			}
			
			auto e = m_context->GetDevice()->CreateDepthStencilView(shadowDepthTexture, &shadowDepthStencilViewDesc, &m_depthStencilView);
			std::cout << "e: " << e << std::endl;


			// �׸��� ���̴� ���ҽ� �� ���� ����ü
			D3D11_SHADER_RESOURCE_VIEW_DESC shadowShaderResourceViewDesc;
			ZeroMemory(&shadowShaderResourceViewDesc, sizeof(shadowShaderResourceViewDesc));
			{
				shadowShaderResourceViewDesc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
				shadowShaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
				shadowShaderResourceViewDesc.Texture2D.MipLevels       = 1;
				shadowShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
			}

			// �׸��� ���̴� ���ҽ� �� ����
			auto c = m_context->GetDevice()->CreateShaderResourceView(shadowDepthTexture, &shadowShaderResourceViewDesc, &m_shaderResourceView);

			std::cout << "c: " << c << std::endl;

			// �ؽ�ó�� ���� ������ �信 ����Ǿ� �����Ƿ� ���� ����
			shadowDepthTexture->Release();
		}

		void ShadowBuffer::BindInternal()
		{
			m_context->GetDeviceContext()->OMSetRenderTargets(0, nullptr, m_depthStencilView);
		}

		void ShadowBuffer::UnBindInternal()
		{
			m_context->GetDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);

			m_context->Bind();
		}
	}
}