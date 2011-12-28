#ifndef __NC_PLT_MATRIXDATA_h__
#define __NC_PLT_MATRIXDATA_h__


namespace nc{

	namespace platform {

		namespace math{

			//ééÇµÇ…ColMajorÇ…ÇµÇƒÇ›Ç‹ÇµÇΩÅB
			struct Matrix33Data : public nc::generic::math::Matrix33ColMajorData{

			};

			struct Matrix44Data : public nc::generic::math::Matrix44ColMajorData{

			};

		}//namespace math

	}//namespace generic

	namespace math{

		using nc::generic::math::Matrix33RowMajorData;
		using nc::generic::math::Matrix33ColMajorData;

		using nc::generic::math::Matrix44RowMajorData;
		using nc::generic::math::Matrix44ColMajorData;

		using nc::platform::math::Matrix33Data;
		using nc::platform::math::Matrix44Data;

	}//namespace math

}//namespace nc

#endif // __NC_PLT_MATRIXDATA_h__