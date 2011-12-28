#ifndef __NC_COORDINATEDATA_h__
#define __NC_COORDINATEDATA_h__

#include "nucl/Types.h"

#include "nucl/Swizzle.h"

namespace nc{

	namespace math{
		class CoordinateHasData_tag {};
		class CoordinateHasData_XY_tag : public CoordinateHasData_tag {};
		class CoordinateHasData_XYZ_tag : public CoordinateHasData_tag {};
		class CoordinateHasData_XYZW_tag : public CoordinateHasData_tag {};
	}

}//namespace nc

#include "Coordinate2Data.h"
#include "Coordinate3Data.h"
#include "Coordinate4Data.h"

#endif // __NC_COORDINATEDATA_h__