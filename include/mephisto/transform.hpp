#pragma once
#ifndef _MEPHISTO_TRANSFORM_HPP_INCLUDED
#define _MEPHISTO_TRANSFORM_HPP_INCLUDED

#include <mephisto/components/components.hpp>
#include <mephisto/math/vector3d/vector3d.h>

namespace mephisto {
	class Transform
	{
	public:
		Transform() {}
		~Transform() {}
		Vec3Df position;
	};
}

#endif