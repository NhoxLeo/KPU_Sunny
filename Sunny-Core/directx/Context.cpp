#include "Context.h"


namespace sunny
{
	namespace directx
	{

#define ReleaseCOM(x)	\
	if (x)				\
	{					\
		x->Release();	\
		x = nullptr;	\
	}

		Context* Context::s_context = nullptr;

		void Context::Create(WindowProperties properties, void* deviceContext)
		{
			s_context = new Context(properties, deviceContext);
		}

		Context::Context(WindowProperties properties, void* deviceContext)
		: m_properties(properties), m_MSAAEnabled(true), m_debugLayerEnabled(true)
		{
			m_renderTargetView   = nullptr;
			m_depthStencilView   = nullptr;
			m_depthStencilBuffer = nullptr;

			/* 0. �ʱ�ȭ�� �����Ѵ�. */
			InitD3D((HWND)deviceContext);
		}

		void Context::InitD3D(HWND hWnd)
		{
			/* 1. ���� D3D11CreateDevice() �Լ��� ���Ͽ� Direct3D ����̽��� ���ؽ�Ʈ�� �����Ѵ� */
			HRESULT hr = D3D11CreateDevice(
				NULL,                        // pAdapter  : ���÷��� ����� ������(2�� �̻��� ����͸� ����� ���) NULL�̸� �� ����͸� ���
				D3D_DRIVER_TYPE_HARDWARE,    // DriverType: �ϵ���� ���� ��� 
				NULL,                        // SoftWare  : ����Ʈ���� ������ -> �ϵ��� ����� ��� NULL
											 // Flags  : D3D11_CREATE_DEVICE_DEBUG          -> ����� ���� Ȱ��ȭ(VC++ ��� â�� ����� �޽���) 
											 //          D3D11_CREATE_DEVICE_SINGLETHREADED -> �̱� ������ �ϰ�� ���� ���, CreateDeffedContext ȣ���� �����Ѵ�.
				m_debugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED,
				NULL,                        // pFeatureLevels : [�迭] ��� ����(NULL �̸� �ְ� ��� ����)
				0,                           // FeatureLevel   : �� �迭�� ���� ���� (NULL �̸� 0���� ����)
				D3D11_SDK_VERSION,           // SDKVersion     : ���� (�׻� D3D11_SDK_VERSION)      
				&dev,                        // ppDevice       : ������ ��ġ�� �����ش�.
				&m_d3dFeatureLevel,          // pFeatureLevel  : ��� ������ �����ش�.
				&devcon                      // ppImmediateContext: ������ ��ġ ���ؽ�Ʈ�� �����ش�.
			);

			// ��Ƽ���ø� ǰ�� ���� ����(�����ϴ� ����Ƽ ����)�� �˾Ƴ���. (���� X �� 0)
			dev->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_MSAAQuality);

			/* 2. CreateSwapChain() �Լ��� ���Ͽ� ���� ü���� �����Ѵ�. */

			// ����ü�� ���� ����ü
			DXGI_SWAP_CHAIN_DESC scd;
			ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
			{
				// BufferDesc: ����� ���� ����
				scd.BufferDesc.Width = m_properties.width;          // ���� ���� ����
				scd.BufferDesc.Height = m_properties.height;        // ���� ���� ����
				scd.BufferDesc.RefreshRate.Numerator = 60;          // ��������(FPS) �и�
				scd.BufferDesc.RefreshRate.Denominator = 1;         // ��������(FPS) ����
				scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ȭ�� ��� ����(���� ����)

																	// SampleDesc: ��Ƽ ���ø� ��� ����
				scd.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;                     // ��Ƽ���ø� ����
				scd.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0; // ��Ƽ���ø� ǰ��

																				  // BufferUsage: �����Ǵ� ������ ����� �����ϴ� �÷���
				scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // ���۰� ����� ���� ������ �������� ���

																	// BufferCount: ������ ������ ����
				scd.BufferCount = 3; // ���� ����

									 // OutputWindow: ����� ������ �ڵ�
				scd.OutputWindow = hWnd;

				// Windowed: ��ü ��� �Ǵ� ������ ���� ��� ������ ����
				scd.Windowed = !m_properties.fullscreen; // true -> ������ ���

				// SwapEffect: swap�� �Ͼ �Ŀ� ������ ������ ��� ���� ����
				scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ������ ������ �� �̻� ������� �ʰ� ������ �ǹ�

				// �߰����� �÷���
				scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //  ���� ������ ���÷��̸��� �ڵ������� ��ȯ
			}

			IDXGIDevice* dxgiDevice = 0;
			IDXGIAdapter* dxgiAdapter = 0;
			IDXGIFactory* dxgiFactory = 0;

			// __uuidof: �������̽��� ���ǵ� GUID�� ������ ������ �Ѵ�.
			dev->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);      // (1) DirectX ��ġ���� IDXGIDevice �� ���´�.
			dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);  // (2) IDXGIDevice ���� ��͸� ���´�.
			dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory); // (3) ��Ϳ��� IDXDIFactory �� ���´�.

																				  // ����ü���� �����Ѵ�.
																				  // (����̽� ������,  ����ü�� ���� ����ü ������, ����ü�� �������̽��� �޾ƿ��� ������)
			dxgiFactory->CreateSwapChain(dev, &scd, &swapchain);

			dxgiFactory->Release();
			dxgiAdapter->Release();
			dxgiDevice->Release();

			/* ���� Ÿ�� �並 �����Ѵ�. */
			Resize();
		}

		void Context::Resize()
		{
			int width  = m_properties.width;
			int height = m_properties.height;

			ReleaseCOM(m_renderTargetView);
			ReleaseCOM(m_depthStencilView);
			ReleaseCOM(m_depthStencilBuffer);

			swapchain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			/* 3. CreateRenderTargetView() �Լ��� ���� ���� Ÿ�� ��(�����)�� �����Ѵ�. */

			ID3D11Texture2D* backBuffer;
			swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

			// ���� Ÿ�� ����
			dev->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
			// pResource: ������ ���� ����ü             �� ����ü���� ����ۿ� ��������
			// pDesc    : ������ ���� ���� ���� ����ü   �� NULL�� �⺻ ��� ������ �ǹ�
			// ppRTView : ������ �並 ��ȯ�޴� ������

			backBuffer->Release();


			/* 4. ���ٽ� �ؽ��ĸ� �����Ѱ� �����Ѵ�. */

			// �ؽ��� ���� ����ü (���ٽ�)
			D3D11_TEXTURE2D_DESC depthStencilDesc;
			{
				depthStencilDesc.Width = width;              // ������ �ؽ����� �ʺ� ����
				depthStencilDesc.Height = height;            // ������ �ؽ����� ���� ����
				depthStencilDesc.MipLevels = 1;              // �ִ� �Ӹ� ������ ���� (�Ӹ�: �� ���� �׸��� ���� ������ ����� ������ �ؽ���)
				depthStencilDesc.ArraySize = 1;              // �ؽ��� �迭�� �ؽ��� ����
				depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  // ����-depth�� 24��Ʈ, ���ٽ� ������ 8����Ʈ�� ����ϴ� ����

				depthStencilDesc.SampleDesc.Count = m_MSAAEnabled ? 4 : 1;                     // ��Ƽ���ø� ����
				depthStencilDesc.SampleDesc.Quality = m_MSAAEnabled ? (m_MSAAQuality - 1) : 0; // ��Ƽ���ø� ǰ��

				// �ؽ����� �б�/���� ����
				depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // �⺻ �б� ����� ������ ���

				// ������(���� ������)���� ���ε��� �����ϴ� �÷���
				depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; // ����/���ٽ� �뵵�� ���� �ǹ�

				// CPU�� ������ �����ϴ� �÷���
				depthStencilDesc.CPUAccessFlags = 0;          // 0�� CPU�� �б�/���⸦ �� �� ������ �ǹ�

				// ��Ÿ ���� ���� �÷���
				depthStencilDesc.MiscFlags = 0;               // 0�� ������� ������ �ǹ�
			}

			// ����/���ٽ� �ؽ��� ���� -> m_depthStencilBuffer
			dev->CreateTexture2D(&depthStencilDesc, 0, &m_depthStencilBuffer);
			// ������ �ؽ��� ���� ����ü
			// �ʱ�ȭ ������(�迭 ������)
			// ������ 2D �ؽ��ĸ� ��ȯ�޴� ������

			// ����/���ٽ� ���� Ÿ�� ����
			dev->CreateDepthStencilView(m_depthStencilBuffer, 0, &m_depthStencilView);
			// pResource: ��� ���� DepthStecil ����
			// pDesc    : ��� ���� DepthStecil ���� ����ü (NULL�� �⺻ ����)
			// ppRTView : ������ DepthStencil�� ��ȯ�޴� ������


			/* 5. ���� Ÿ���� ����̽� ���ؽ�Ʈ�� ��� ���� �ܰ迡 �����Ѵ�. */
			devcon->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
			// NumViews : ���� ���� (���� 1)
			// ppRenderTargetViews: ���� ���� ������        (CreateRenderTargetView ���� ����)
			// pDepthStencilView : DepthStencil ���� ������ (CreateDepthStencilView ���� ����)


			/* 6. ����Ʈ(�����쿡 �۹��� �׷����� ����)�� �����ϰ� �����Ѵ�. */
			m_screenViewport.TopLeftX = 0;
			m_screenViewport.TopLeftY = 0;
			m_screenViewport.Width = (float)width;
			m_screenViewport.Height = (float)height;
			m_screenViewport.MinDepth = 0.0f;
			m_screenViewport.MaxDepth = 1.0f;
			devcon->RSSetViewports(1, &m_screenViewport);

			/* 7. �����Ͷ������� �����ϰ� �����Ѵ�.(������ ��� �׷��������� ���� ����) &*/
			D3D11_RASTERIZER_DESC rasterDesc;
			{
				// ��� ����� ���� ���̹Ƿ� �׸� ���� ������ �ʴ´�.
				// �ڼ��� ������ SDK ������ ���� �ٶ��ϴ�.
				rasterDesc.AntialiasedLineEnable = false;
				rasterDesc.CullMode = D3D11_CULL_NONE;         // NONE: �ﰢ�� ���� ��Ȱ��ȭ, BACK: �ĸ� �ﰢ�� ����, FRONT: ���� �ﰢ�� ����
				rasterDesc.DepthBias = 0;
				rasterDesc.DepthBiasClamp = 0.0f;
				rasterDesc.DepthClipEnable = true;
				rasterDesc.FillMode = D3D11_FILL_SOLID;        // ���̾� ������ or �ָ��� ������
				rasterDesc.FrontCounterClockwise = true;      // �ð� ���� �ﰢ�� ����(true), �ݽð� ���� �ﰢ�� ����(false)
				rasterDesc.MultisampleEnable = false;
				rasterDesc.ScissorEnable = false;
				rasterDesc.SlopeScaledDepthBias = 0.0f;
			}

			ID3D11RasterizerState* rs;
			dev->CreateRasterizerState(&rasterDesc, &rs);
			devcon->RSSetState(rs);

			ReleaseCOM(rs);
		}

		void Context::Present()
		{
			// ���� ���۸��� ����� �� �ĸ� ������ ������ ���� ���۷� �����ϴ� ���� ���������̼��̶�� �Ѵ�.
			swapchain->Present(m_properties.vsync, 0);
		}
	}
}