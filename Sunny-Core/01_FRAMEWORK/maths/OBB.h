//
// Created by adunstudio on 2018-04-21.
//
#pragma once

#include "../include.h"
#include "vec3.h"

namespace sunny
{
	namespace maths
	{
		struct OBB
		{
			vec3 center;        // �߾�
			vec3 axis;          // �� ���� ���� ����
			vec3 len;           // �� ���� ����
		
			OBB(const vec3& center, const vec3& axis, const vec3& len);

			bool Intersects(const OBB& other) const;
		};
	}
}