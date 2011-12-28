#ifndef __NC_PLT_MATRIXUTIL_h__
#define __NC_PLT_MATRIXUTIL_h__

namespace nc{

	//-------------------------------------------------

	namespace platform{

		namespace math{
			nc::math::Vector2 transform( const nc::math::Matrix33& mtx, const nc::math::Vector2& vec );	//Z = 0として計算されます。
			nc::math::Vector3 transform( const nc::math::Matrix33& mtx, const nc::math::Vector3& vec );

			nc::math::Vector3 transform( const nc::math::Matrix44& mtx, const nc::math::Vector3& vec );	//W = 0として計算されます。
			nc::math::Vector4 transform( const nc::math::Matrix44& mtx, const nc::math::Vector4& vec );

			nc::math::Point2 transform( const nc::math::Matrix33& mtx, const nc::math::Point2& pt );		//Z = 1として計算されます。
			nc::math::Point3 transform( const nc::math::Matrix33& mtx, const nc::math::Point3& pt );

			nc::math::Point3 transform( const nc::math::Matrix44& mtx, const nc::math::Point3& pt );		//W = 1として計算されます。
			nc::math::Point4 transform( const nc::math::Matrix44& mtx, const nc::math::Point4& pt );
			
		}//namespace math

	}//namespace platform


	namespace math{

		using nc::generic::math::matrix33RotateAxisX;
		using nc::generic::math::matrix33RotateAxisY;
		using nc::generic::math::matrix33RotateAxisZ;
		using nc::generic::math::matrix33RotateAxis;

		using nc::generic::math::matrix33RotateYawPitchRoll;
		using nc::generic::math::matrix33RotateQuaternion;
		using nc::generic::math::quaternionRotateMatrix;
		using nc::generic::math::matrix33Scale;
		using nc::generic::math::matrix44RotateAxisX;
		using nc::generic::math::matrix44RotateAxisY;
		using nc::generic::math::matrix44RotateAxisZ;
		using nc::generic::math::matrix44RotateAxis;
		using nc::generic::math::matrix44Scale;
		using nc::generic::math::matrix44Translate;
		using nc::generic::math::matrix44LookAt;
		using nc::generic::math::matrix44Ortho;
		using nc::generic::math::matrix44OrthoOffCenter;
		using nc::generic::math::matrix44Perspective;
		using nc::generic::math::matrix44PerspectiveOffCenter;
		using nc::generic::math::matrix44PerspectiveFov;
		using nc::platform::math::transform;

	}//namespace math

}//namespace nc

#endif // __NC_PLT_MATRIXUTIL_h__