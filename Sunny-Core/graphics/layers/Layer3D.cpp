#include "Layer3D.h"

#include "../cameras/MayaCamera.h"

namespace sunny
{
	namespace graphics
	{
		Layer3D::Layer3D(Renderer3D * renderer)
		: m_renderer(renderer), m_camera(new MayaCamera(maths::mat4::Perspective(65.0f, 16.0f / 9.0f, 0.1f, 1000.0f)))
		{

		}

		Layer3D::~Layer3D()
		{
			for (unsigned int i = 0; i < m_renderables.size(); ++i)
				delete m_renderables[i];

			m_renderables.clear();

			delete m_camera;
		}

		Renderable3D* Layer3D::Add(Renderable3D* renderable)
		{
			m_renderables.push_back(renderable);

			return renderable;
		}

		void Layer3D::SetCamera(Camera * camera)
		{
			delete m_camera;

			m_camera = camera;

			m_camera->Focus();
		}

		void Layer3D::Init()
		{
			OnInit(*m_renderer);
		}
		
		void Layer3D::OnInit(Renderer3D& renderer)
		{

		}

		void Layer3D::OnUpdateInternal(const utils::Timestep& ts)
		{
			OnUpdate(ts);
		}

		void Layer3D::OnRender(Renderer3D& renderer)
		{
			
		}

		void Layer3D::OnRender()
		{
			m_camera->Update();

			m_renderer->Begin();
			m_renderer->BeginScene(m_camera);

			for (Renderable3D* renderable : m_renderables)
			{
				m_renderer->Submit(renderable);
			}

			m_renderer->EndScene();
			m_renderer->End();

			m_renderer->Present();

			OnRender(*m_renderer);
		}

		bool Layer3D::OnResize(unsigned int width, unsigned int height)
		{
			m_renderer->SetScreenBufferSize(width, height);
			return false;
		}
	}
}