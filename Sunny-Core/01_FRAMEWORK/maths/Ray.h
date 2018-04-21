//
// Created by adunstudio on 2018-04-21.
//

#pragma once

#include "../include.h"
#include "vec3.h"
#include "vec2.h"
#include "AABB.h"
#include "OBB.h"

namespace sunny
{
	namespace maths
	{
		struct Ray
		{
			vec3 pos;    // ������ ����
			vec3 dir;    // ������ ���� (�븻)

			Ray();
			Ray(const vec3& pos, const vec3& dir);

			bool Intersect(const Ray& other);
			bool Intersect(const AABB& other);
			bool Intersect(const OBB& other);
			
			friend std::ostream& operator<<(std::ostream& stream, const Ray& ray);
		};
	}
}
