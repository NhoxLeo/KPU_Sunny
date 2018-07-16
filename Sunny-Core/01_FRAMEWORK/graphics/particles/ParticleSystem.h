#pragma once
#include "../../include.h"
#include "../../maths/maths.h"
#include "../../directx/Texture2D.h";
#include "../../directx/VertexBuffer.h"
#include "../../directx/IndexBuffer.h"
#include "../../directx/VertexArray.h"
#include "../../directx/Shader.h"

namespace sunny
{
	namespace graphics
	{
		struct Particle
		{
			maths::vec3 position;
			maths::vec4 color;
			float velocity;
			bool active;
		};

		struct ParticleVertex
		{
			maths::vec3 position;          // ��ġ
			maths::vec4 color;             // ����
			maths::vec2 uv;                // �ؽ�ó ��ġ
		};


		class ParticleSystem
		{
		protected:
			directx::Texture* m_texture;                      // �ؽ�ó 

			directx::VertexArray*  m_vertexArray;
			directx::VertexBuffer* m_vertexBuffer;
			directx::IndexBuffer*  m_indexBuffer;

			Particle* m_particleList;
			ParticleVertex* m_particleVertices;

			unsigned int m_vertexCount;
			unsigned int m_indexCount;

		protected:
			int m_currentParticleCount;

			unsigned int m_maxParticleCount;

			maths::vec3 m_position;                           // ��ġ
			maths::vec3 m_rotate;                             // ȸ��
			maths::vec3 m_speed;                              // �ӵ�
			maths::vec3 m_accelaration;                       // ���ӵ�
			maths::vec4 m_color;                              // �÷�
			
			float m_size = 20.0f;                             // ũ�� 
			float m_fade;                                     // ��� ����
			float m_age;                                      // ����
			
			float m_accumulatedTime = 0;                      // ��ƼŬ�� ����Ǵ� �ӵ� ���� �ð�
			float m_particlePerSecond = 650.0f;               // �ʴ� ���� �� ��ƼŬ ��
			float m_lifeTime;                                 // ��ƼŬ�� �Ҹ�ɶ����� �����Ǵ� �ð�

			bool m_visible;                                
		
		protected:
			void KillParticles();
			void EmitParticles(const utils::Timestep& ts);
			void UpdateBuffers();

		public:
			ParticleSystem(directx::Texture* texture);
			~ParticleSystem();
			
			virtual void Update(const utils::Timestep& ts);
			virtual void Render();

		public:
			inline const maths::vec3& GetPosition() const { return m_position; }
			inline void SetPosition(const maths::vec3& position) { m_position = position; }

			inline const maths::vec3& GetSpeed() const { return m_speed; }
			inline void SetSpeed(const maths::vec3& speed) { m_speed = speed; }

			inline const maths::vec3& GetAccelaration() const { return m_accelaration; }
			inline void SetAccelaration(const maths::vec3& accelaration) { m_accelaration = accelaration; }

			inline const maths::vec4& GetColor() const { return m_color; };
			inline void SetColor(const maths::vec4& color) { m_color = color; };

			inline const float GetSize() const { return m_size; }
			inline void SetSize(const float size){ m_size = size; }

			inline const float GetFade() const { return m_fade; }
			inline void SetFade(const float fade) { m_fade = fade; }

			inline const float GetLifeTime() const { return m_lifeTime; }
			inline void SetLifeTime(const float lifeTime) { m_lifeTime = lifeTime; }

			inline const float GetAge() const { return m_age; }
			inline void SetAge(const float age) { m_age = age; }

			inline const bool GetVisible() const { return m_visible; };
			inline void SetVisible(bool visible) { m_visible = visible; };
		};
	}

	
}