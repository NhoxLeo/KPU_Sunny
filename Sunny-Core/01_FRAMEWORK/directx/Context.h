#pragma once

#include "../include.h"
#include "Common.h"
#include "../app/Window.h"

/*
DirectX �׷��� ���α׷��� �⺻������ ������ ���� �������̽� ��ü�� �ʿ��ϴ�.

- 1. Direct3D ����̽�(Device)         �������̽�
- 2. ����̽� ���ؽ�Ʈ(Device Context) �������̽�
- 3. ���� ü��(Swap Chain)             �������̽�
- 4. ���� Ÿ�� ��(Render Target View)  �������̽�

�⺻������ Direct3D ����̽� �������̽��� ����(���� ����, �ε��� ����, ��� ���� ��)�� ����, ���ҽ�(�ؽ�ó ��)�� ����, ���̴� ����, �����Ͷ����� ���� ����, ���ҽ� ���� ���� ���� �ʿ��ϴ�.

���� ü�� �������̽��� �ĸ� ������ ������ ����ϱ� ����, �� ���������̼��� �ϴµ� �ʿ��ϴ�.

����̽� ���ؽ�Ʈ �������̽��� �������� ���õ� �۾��� ���� �ʿ��ϴ�.
*/
namespace sunny
{
	namespace directx
	{
		class Context
		{
		private:
			static Context* s_context;

		public:
			static void Create(WindowProperties properties, void* deviceContext);

		public:
			// ����̽� �������̽��� ���� �����ʹ�.  �ַ� ���ҽ��� �����ϴ� �� �ʿ��ϴ�.
			ID3D11Device* dev;

			// ����̽� ���ؽ�Ʈ�� ���� �����ʹ�.    �ַ� ������������ �����ϴ� �� �ʿ��ϴ�.
			ID3D11DeviceContext* devcon[2];

			// ���� ü�� �������̽��� ���� �����ʹ�. �ַ� ���÷��̸� �����ϴ� �� �ʿ��ϴ�.
			IDXGISwapChain* swapchain;

		private:
			D3D_FEATURE_LEVEL m_d3dFeatureLevel; // ��� ���� ����
			unsigned int  m_MSAAQuality;         // ��Ƽ���ø� ǰ�� ����
			bool m_MSAAEnabled;                  // ��Ƽ���ø� ǰ�� ���� ����
			bool m_debugLayerEnabled;            // ����� ������ Ȱ��ȭ �ϸ� Direct3D�� VC++ ���â�� ����� �޽����� ������.

			ID3D11Texture2D* m_texture;

			ID3D11RenderTargetView* m_renderTargetView; // ȭ�鿡 �������� ���� ��ü(���� Ÿ�� ��) �������̽�(swapchain���� �����ǰ� �����ȴ�.)
			ID3D11DepthStencilView* m_depthStencilView; // ����/���ٽ� ������ ������� View�� �����ϴ� ��ü�� �������̽�
			
			D3D11_VIEWPORT m_screenViewport;            // ����Ʈ(�����쿡 �׸��� �׷����� ������  ����)

			ID3D11Texture2D* m_depthStencilBuffer;      // �Ϲ����� 2D �̹����� �����ϴ� ��ü �������̽�(���⼭�� ����/���ٽ� ���� �����)

			WindowProperties m_properties;

			ID3D10Multithread* m_lock;
		public:
			Context(WindowProperties properties, void* deviceContext);
						
			void Present3D();

			void Present();

		private:		
			void InitD3D(HWND hWnd);

			void Resize();

			void BindInternal();

		public:
			/* ���� */
			inline static Context* GetContext() { return s_context; }
			inline static Context* Get       () { return s_context; }

			inline static IDXGISwapChain* GetSwapChain() { return GetContext()->swapchain; }
			inline static ID3D11Device* GetDevice() { return GetContext()->dev; }
			inline static ID3D11DeviceContext* GetDeviceContext(int dimension = DIMENSION::D3) { return GetContext()->devcon[dimension]; }
			inline static ID3D11RenderTargetView* GetBackBuffer() { return GetContext()->m_renderTargetView; }
			inline static ID3D11DepthStencilView* GetDepthStencilBuffer() { return GetContext()->m_depthStencilView; }

			inline static const WindowProperties& GetWindowProperties() { return GetContext()->m_properties; }

			inline static void Bind() { return GetContext()->BindInternal(); };
		};
	}
}