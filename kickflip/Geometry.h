#pragma once

#include "nucl/math/Vector.h"

namespace kickflip
{

	namespace Geometry 
	{
		using namespace nc::math;
		struct Line
		{
			Vector3 p;
			Vector3 v;
		};
		struct Plane
		{
			Vector4 pl;
		};
	}

}

