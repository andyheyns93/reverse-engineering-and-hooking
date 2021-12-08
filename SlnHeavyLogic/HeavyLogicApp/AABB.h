#pragma once
#include "Vec3.h"

class AABB
{
private:
	Vec3 min;
	Vec3 max;
public:
	AABB(const Vec3& vmin, const Vec3& vmax);
	bool IsContainPoint(const Vec3& pos);
};

