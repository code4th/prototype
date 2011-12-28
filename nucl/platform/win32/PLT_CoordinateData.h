#ifndef __NC_PLT_COORDINATEDATA_h__
#define __NC_PLT_COORDINATEDATA_h__

namespace nc{

	namespace platform{

		namespace math{

			__declspec(align(16)) struct Coordinate2Data{
				union{
					real_t idx[ 2 ];
					struct{
						real_t x, y;
					};
				};		
			};

			__declspec(align(16)) struct Coordinate3Data{
				union{
					real_t idx[ 3 ];
					struct{
						real_t x, y, z;
					};
				};
			};

			__declspec(align(16)) struct Coordinate4Data{
				union{
					real_t idx[ 4 ];
					struct{
						real_t x, y, z, w;
					};
				};		
			};

		}//namespace math

	}//namespace platform

	namespace math{

		//•K—v‚É‰‚¶‚Ä–¼‘O‚ğÚ‚¹‘Ö‚¦‚Ä‚­‚¾‚³‚¢
		using nc::platform::math::Coordinate2Data;
		using nc::platform::math::Coordinate3Data;
		using nc::platform::math::Coordinate4Data;

	}//namespace math

}//namespace nc


#endif // __NC_PLT_COORDINATEDATA_h__