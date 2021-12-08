#include "AABB.h"

AABB::AABB(const Vec3& vmin, const Vec3& vmax)
{
	this->min = vmin;
	this->max = vmax;
}

bool AABB::IsContainPoint(const Vec3& pos)
{
	if (pos.x < min.x) return false;
	if (pos.y < min.y) return false;
	if (pos.z < min.z) return false;
	if (pos.x > max.x) return false;
	if (pos.y > max.y) return false;
	if (pos.z > max.z) return false;
	return true;
}