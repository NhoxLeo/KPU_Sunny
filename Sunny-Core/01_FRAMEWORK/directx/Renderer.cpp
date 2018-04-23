#include "Renderer.h"

namespace sunny
{
	namespace directx
	{
		Renderer* Renderer::s_instance = nullptr;

		std::vector<ID3D11BlendState*>        Renderer::s_blendStates;
		std::vector<ID3D11DepthStencilState*> Renderer::s_depthStencilStates;

		Renderer::Renderer()
		{
			m_context = Context::GetContext();
		}

		void Renderer::Init()
		{
			s_instance = new Renderer();  
			s_instance->InitInternal(); 
		}

		void Renderer::InitInternal()
		{
			CreateBlendStates();
			CreateDepthStencilStates();

			SetDepthTesting(false);
			SetBlend(true);
		}

		void Renderer::ClearInternal(unsigned int buffer)
		{
			float color[4] = { 0.87f, 1.f, 1.f, 1.0f }; // ��

			if (buffer & RendererBufferType::RENDERER_BUFFER_COLOR)
				Context::GetDeviceContext()->ClearRenderTargetView(Context::GetBackBuffer(), color);

			if (buffer & RendererBufferType::RENDERER_BUFFER_DEPTH)
				Context::GetDeviceContext()->ClearDepthStencilView(Context::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
			if (buffer & RendererBufferType::RENDERER_BUFFER_DEFERRED)
			{
				Context::GetDeviceContext()->ClearRenderTargetView(GeometryBuffer::GetRenderTargeBuffer(GeometryTextureType::DIFFUSE), color);
				Context::GetDeviceContext()->ClearRenderTargetView(GeometryBuffer::GetRenderTargeBuffer(GeometryTextureType::NORMAL), color);
			}

		
			if (buffer & RendererBufferType::RENDERER_BUFFER_SHADOW)
				Context::GetDeviceContext()->ClearDepthStencilView(GeometryBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			
			
			if (buffer & RendererBufferType::RENDERER_BUFFER_DEBUG)
			{
				Context::GetDeviceContext()->ClearRenderTargetView(DebugBuffer::GetBuffer(), color);
				Context::GetDeviceContext()->ClearDepthStencilView(DebugBuffer::GetDepthStencilBuffer(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
		}

		void Renderer::PresentInternal()
		{
			m_context->Present();
		}

		// ����/���ٽ� ���¸� ��� ���ձ� �ܰ迡 ���´�.
		void Renderer::SetDepthTestingInternal(bool enabled)
		{
			Context::GetDeviceContext()->OMSetDepthStencilState(enabled ? s_depthStencilStates[0] : s_depthStencilStates[1], NULL);
		}

		// ȥ�� ���¸� ��� ���ձ� �ܰ迡 ���´�.
		void Renderer::SetBlendInternal(bool enabled)
		{
			Context::GetDeviceContext()->OMSetBlendState(enabled ? s_blendStates[1] : s_blendStates[0], NULL, 0xffffffff);
		}

		// ȥ�� ���� �� ���� ����
		void Renderer::CreateBlendStates()
		{
			// ȥ�� �������� ���� ������ �ϳ��� ȥ�� ����(blendstate)�� ��Ÿ����
			// ID3D11BlendState �������̽��� �����Ѵ�.
			// �� �������̽��� �������� D3D11_BLEND_DESC ����ü�� ä���
			// ID3D11Device::CreateBlendState�޼��带 ȣ���ؾ� �Ѵ�.

			// enabled : false
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

				desc.RenderTarget[0].BlendEnable = false;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				ID3D11BlendState* state;
				Context::GetDevice()->CreateBlendState(&desc, &state);

				s_blendStates.push_back(state);
			}

			// enabled : true
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
				
				desc.AlphaToCoverageEnable  = false;  
				// true�� �����ϸ� �ؽ�ó�� �������� ������ ����ǥ��ȭ ����� ����-������(alpha-to-coverage) ��ȯ�� Ȱ��ȭ �ȴ�.
				// ����-������ ��ȯ�� ���ؼ��� ����ǥ��ȭ�� Ȱ��ȭ�ؾ� �Ѵ�.(����ȭ ǥ�Ϳ� �´� �ĸ� ���ۿ� ���� ���۸� �����ؾ��Ѵ�.)
				
				desc.IndependentBlendEnable = false;
				// Direct3D 11������ ����� �ִ� 8�� ������ ���� ���鿡 ���ÿ� �������� �� �ֵ�.
				// ture�� �����ϸ� �� ���� ��󸶴� ȥ���� ���������� ������ �� �ִ�.
				// false�� �ϸ� ��� ���� ����� ȥ�� ������ D3D11_BLEND_DSSC::RenderTarget �迭�� ù ���ҿ� �ִ� ������ ���� �����ϰ� ����ȴ�.

				// RenderTarget::D3D11_RENDER_TARGET_BLEND_DESC ���� ���� ��¥�� �迭��,
				// �� �迭�� i��° ���Ҵ� ���� ���� ����� i��° ���� ��� ������ ȥ�� ������ ���� ����ü��.
				// IndependentBlendEnable�� false�� �����ߴٸ� ��� ���� ����� RenderTarget[0]�� ȥ���� ����Ѵ�.

				desc.RenderTarget[0].BlendEnable = true; // ȥ���� Ȱ��ȭ �Ϸ��� true
				
				desc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;     // RGB ���� ȥ���� ���� ȥ�� ���
				desc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA; // RGB ���� ȥ���� ��� ȥ�� ���
				desc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;        // RGB ���� ȥ�� ������
				desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;     // ���� ���� ȥ���� ���� ȥ�� ���
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;     // ���� ���� ȥ���� ���� ȥ�� ���
				desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;        // ���� ���� ȥ�� ������

				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // ���� ��� ���� ����ũ

		
				ID3D11BlendState* state;
				Context::GetDevice()->CreateBlendState(&desc, &state);

				s_blendStates.push_back(state);
			}
		}

		// ����/���ٽ� ���� �� ���� ����
		void Renderer::CreateDepthStencilStates()
		{
			// ���ٽ� ���۸� �̿��ϸ� �ĸ� ������ Ư�� ���������� �������� ������ �� �ִ�.
			// �־��� �� �ȼ��� �ĸ� ���ۿ� ����� ������ �ƴ����� ���ٽ� ���ۿ� �ٰ��ؼ� �����ϴ� ���� ���ٽ� ����(stencil test)��� �θ���.

			// ���ٽ� ���۴�(+���� ����) ID3D11DepthStencilState��� ���� ���� �������̽��� �����Ѵ�.
			// ID3D11DepthStencilState �������̽��� �����Ϸ��� �켱 D3D11_DEPTH_STENCIL_DESC ����ü�� ä���� �Ѵ�.
			
			// enabled : true
			{
				D3D11_DEPTH_STENCIL_DESC desc;
				
				desc.DepthEnable    = true;                                     // ���� ������ Ȱ��ȭ(true), ��Ȱ��ȭ(false)

				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;               // ZERO: ���� ���� ����(����) ����, ���� ���� ���� | ALL: ���� ���� ����(����)�� Ȱ��ȭ 
				desc.DepthFunc      = D3D11_COMPARISON_LESS;              // ���� ������ ���̴� �� �Լ�, D3D11_COMPAROSION_FUNC �������� �� ���
				
				desc.StencilEnable    = true;                                   // ���ٽ� ���� Ȱ��ȭ(true), ��Ȱ��ȭ(false)
				desc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;        // ���ٽ� ���� ���ǹ�   // �⺻���� �� � ��Ʈ�� ���� �ʴ� ����ũ
				desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;       // ���ٽ� ���۸� �����Ҷ� Ư�� ��Ʈ ������ ���ŵ��� �ʵ��� �ϴ� ��Ʈ ����ũ // �⺻���� �� � ��Ʈ�� ���� �ʴ� ����ũ 

				/* FrontFace: ���� �ﰢ���� ���� ���ؽ� ���� ���� ��� */
				desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;      // �ȼ� ������ ���ؽ� ������ �������� ���� ���ٽ� ������ ���� ���
				desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;      // �ȼ� ������ ���ٽ� ������ ����Ǿ����� ���� ������ �������� ���� ���ٽ� ������ ���� ���
				desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;  // �ȼ� ������ ���ٽ� ������ ���� ������ ��� ������� ���� ���ٽ� ������ ���� ��� 
				desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;    // ���ٽ� ���� �� �Լ��� �����ϴ� ��� 

				/* BackFace: �ĸ� �ﰢ���� ���� ���ؽ� ���� ���� ��� */
				desc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_DECR;
				desc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;

				// D3D11_STENCIL_OP_KEEP     : ���ٽ� ���۸� �������� �ʵ��� �Ѵ�. ��, ���� �մ� ���� �����Ѵ�.
				// D3D11_STENCIL_OP_INCR_SAT : ���ٽ� ���� �׸��� �����Ѵ�. �ִ��� �ѱ�� �ִ����� �����Ѵ�. 
				
				ID3D11DepthStencilState* state;
				Context::GetDevice()->CreateDepthStencilState(&desc, &state);
			
				s_depthStencilStates.push_back(state);
			}
			
			// enabled : false
			{
				D3D11_DEPTH_STENCIL_DESC desc;

				desc.DepthEnable = false;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

				desc.StencilEnable = true;
				desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;       
				desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

				desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
				desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
				desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
				desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

				ID3D11DepthStencilState* state;
				Context::GetDevice()->CreateDepthStencilState(&desc, &state);

				s_depthStencilStates.push_back(state);
			}
		}
	}
}