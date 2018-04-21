//
// Created by adunstudio on 2018-01-07.
//
#pragma once

#include "../include.h"
#include "vec2.h"
#include "vec3.h"

namespace sunny
{
    namespace maths
    {
        struct Rectangle;

        struct AABB
        {
            vec3 min;
            vec3 max;

            AABB();
            AABB(const Rectangle& rectangle);
            AABB(const vec2& min, const vec2& max);
            AABB(const vec3& min, const vec3& max);
            AABB(float x, float y, float width, float height);
            AABB(float x, float y, float z, float width, float height, float depth);

            bool Intersects(const AABB& other) const;
            bool Contains(const vec2& point) const;
            bool Contains(const vec3& point) const;

            vec3 Center() const;

            bool operator==(const AABB& other) const;
            bool operator!=(const AABB& other) const;

            bool operator<(const AABB& other) const;
            bool operator>(const AABB& other) const;

            friend std::ostream& operator<<(std::ostream& stream, const AABB& AABB);

            inline vec3 GetSize() const { return vec3(abs(max.x - min.x), abs(max.y - min.y), abs(max.z - min.z)); }
        };
    }
}