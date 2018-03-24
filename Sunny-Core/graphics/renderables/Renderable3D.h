#pragma once

#include "../../maths/maths.h"
#include "../../component/Components.h"
#include "../meshs/Mesh.h"
#include "../meshs/MeshFactory.h"
#include "../materials/MaterialInstance.h"
#include "../../directx/Texture2D.h";
#include "../../directx/VertexBuffer.h"
#include "../../directx/IndexBuffer.h"
#include "../../directx/Shader.h"

namespace sunny
{
	namespace graphics
	{
		class Renderable3D
		{
		public:
			enum class RenderFlags
			{
				DISABLE_DEPTH_TEST = BIT(0),
				WIREFRAME = BIT(1)
			};

		protected:
			Mesh* m_mesh;
			directx::Texture* m_texture;
			maths::vec4 m_color;

			int  m_renderFlags;
			bool m_visible;

			MaterialInstance* m_materialInstance;

		protected:
			std::unordered_map<component::ComponentType*, component::Component*> m_components;

		protected:
			Renderable3D(const maths::mat4& transform);

		public:
			void AddComponent(component::Component* component);

			template <typename T>
			const T* GetComponent() const
			{
				return GetComponentInternal<T>();
			}

			template <typename T>
			T* GetComponent()
			{
				return (T*)GetComponentInternal<T>();
			}

			inline directx::Shader* GetShader() const { return m_materialInstance ? m_materialInstance->GetMaterial()->GetShader() : nullptr; };

			inline const maths::vec4& GetColor()     const { return m_color;     }
			inline const float& GetHasTexture() const { return m_texture ? 1.0f : 0.0f; }

			inline const bool GetVisible() const { return m_visible; };
			inline void SetVisible(bool visible) { m_visible = visible; };			

			inline void SetMaterial(MaterialInstance* materialInstance) { m_materialInstance = materialInstance; }
			inline MaterialInstance* GetMaterialInstance() const { return m_materialInstance; }

			inline Mesh* GetMesh() const { return m_mesh; }

			virtual void Render() = 0;

		private:
			template <typename T>
			const T* GetComponentInternal() const
			{
				component::ComponentType* type = T::GetStaticType();

				auto it = m_components.find(type);
				
				if (it == m_components.end())
					return nullptr;

				return (T*)it->second;
			}

		public:
			inline component::TransformComponent* GetTransformComponent() { return GetComponent<component::TransformComponent>(); }

		};
	}
}