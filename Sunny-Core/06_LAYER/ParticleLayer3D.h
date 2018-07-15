#pragma once

#include <iostream>
#include <SUNNY.h>

#include "MouseLayer2D.h"
#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/graphics/Animation3D.h"
#include "../05_GAME/graphics/Bullet.h"

using namespace std;
using namespace sunny;
using namespace graphics;
using namespace directx;
using namespace events;
using namespace maths;
using namespace game;

class ParticleLayer3D : public Layer3D
{
private:
	ParticleSystem * m_particle;
	MaterialInstance* m_SkyboxMaterial;

	vector<Mesh*> m_animations;
	vector<Mesh*> m_boss_animations;

	Animation3D* m_player;
	Animation3D* m_boss;

public:
	ParticleLayer3D();
	~ParticleLayer3D();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
	bool OnMouseMovedEvent(MouseMovedEvent& event);
};