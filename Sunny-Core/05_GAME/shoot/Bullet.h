#pragma once
#include <SUNNY.h>

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace maths;

namespace game
{
	enum BULLET_TYPE
	{
		DIRECTIONAL,
		SPIRAL
	};

	class Bullet
	{
	private:
		const int MAX_X = 1500;
		const int MAX_Z = 1500;

	private:
		maths::vec3 m_position;
		
		float m_speed;
		float m_angle;

		float m_speedRate;
		float m_angleRate;
		
		float m_totalMovement;

	public:
		bool alive;

		BULLET_TYPE type;

	public:
		Bullet(BULLET_TYPE type, const maths::vec3& position, float angle, float angleRate, float speed, float speedRate);
		~Bullet();

		virtual void Update(float elapsedTime);

	public:
		inline const maths::vec3& GetPosition() const { return m_position; }
	};
}