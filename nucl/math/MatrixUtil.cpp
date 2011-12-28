
#include "MatrixUtil.h"
#include "VectorUtil.h"

namespace nc{

		//-------------------------------------------------
	namespace generic{

		namespace math{

			using nc::math::cos;
			using nc::math::sin;
			using nc::math::tan;
			using nc::math::pow2;
			using nc::math::sqrt;

			//using nc::math::Matrix33;
			//using nc::math::Matrix44;

			nc::math::Matrix33 matrix33RotateAxisX( real_t rad )
			{
				real_t c = cos( rad );
				real_t s = sin( rad );

				return Matrix33(
					1,	0,	0,
					0,	c,	-s,
					0,	s,	c 
					);
			}

			nc::math::Matrix33 matrix33RotateAxisY( real_t rad )
			{
				real_t c = cos( rad );
				real_t s = sin( rad );

				return nc::math::Matrix33(
					c,	0,	s,
					0,	1,	0,
					-s,	0,	c
					);
			}

			nc::math::Matrix33 matrix33RotateAxisZ( real_t rad )
			{
				real_t c = cos( rad );
				real_t s = sin( rad );

				return nc::math::Matrix33(
					c,	-s,	0,
					s,	c,	0,
					0,	0,	1 
					);
			}

			//axisは単位ベクトルである必要がある。
			nc::math::Matrix33 matrix33RotateAxis( const nc::math::Vector4& axis, real_t rad )
			{
				real_t c = cos( rad );
				real_t s = sin( rad );
				real_t invc = 1 - c;

				return nc::math::Matrix33(
					c + nc::math::pow2(axis.x) * invc,
					axis.x * axis.y * invc - axis.z * s,
					axis.x * axis.z * invc + axis.y * s,

					axis.x * axis.y * invc + axis.z * s,
					c + nc::math::pow2(axis.y) * invc,
					axis.y * axis.z * invc + axis.x * s,

					axis.x * axis.z * invc + axis.y * s,
					axis.y * axis.z * invc + axis.x * s,
					c + nc::math::pow2(axis.z) * invc
					);
			}
		

			nc::math::Matrix33 matrix33RotateYawPitchRoll( real_t yaw, real_t pitch, real_t roll )
			{
				return matrix33RotateAxisY( yaw ) * matrix33RotateAxisX( pitch ) * matrix33RotateAxisZ( roll );
			}

			nc::math::Matrix33 matrix33RotateQuaternion( const nc::math::Quaternion& q )
			{
				return nc::math::Matrix33(
					1 - (2 * pow2(q.y)) - (2 * pow2(q.z)),		(2 * q.x * q.y) - (2 * q.w * q.z),		(2 * q.x * q.z) + (2 * q.w * q.y),
					(2 * q.x * q.y) + (2 * q.w * q.z),			1 - (2 * pow2(q.x)) - (2 * pow2(q.z)),	(2 * q.y * q.z) - (2 * q.w * q.x),
					(2 * q.x * q.z) - (2 * q.w * q.y),			(2 * q.y * q.z) + (2 * q.w * q.x),		1 - (2 * pow2(q.x)) - (2 * pow2(q.y))
					);
			}

			nc::math::Quaternion quaternionRotateMatrix( const nc::math::Matrix33& mtx )
			{
				//http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixTonc::math::Quaternion/index.htm

				nc::math::Quaternion q;

				real_t trace = mtx.m[0][0] + mtx.m[1][1] + mtx.m[2][2];
				if( trace > 0 )
				{
					real_t s = 0.5f / sqrt(trace+ 1.0f);
					q.w = 0.25f / s;
					q.x = ( mtx.m[2][1] - mtx.m[1][2] ) * s;
					q.y = ( mtx.m[0][2] - mtx.m[2][0] ) * s;
					q.z = ( mtx.m[1][0] - mtx.m[0][1] ) * s;
				}
				else
				{
					if ( mtx.m[0][0] > mtx.m[1][1] && mtx.m[0][0] > mtx.m[2][2] )
					{
						real_t s = 2.0f * sqrt( 1.0f + mtx.m[0][0] - mtx.m[1][1] - mtx.m[2][2]);
						q.w = (mtx.m[2][1] - mtx.m[1][2] ) / s;
						q.x = 0.25f * s;
						q.y = (mtx.m[0][1] + mtx.m[1][0] ) / s;
						q.z = (mtx.m[0][2] + mtx.m[2][0] ) / s;
					}
					else if (mtx.m[1][1] > mtx.m[2][2])
					{
						real_t s = 2.0f * sqrt( 1.0f + mtx.m[1][1] - mtx.m[0][0] - mtx.m[2][2]);
						q.w = (mtx.m[0][2] - mtx.m[2][0] ) / s;
						q.x = (mtx.m[0][1] + mtx.m[1][0] ) / s;
						q.y = 0.25f * s;
						q.z = (mtx.m[1][2] + mtx.m[2][1] ) / s;
					}
					else
					{
						real_t s = 2.0f * sqrt( 1.0f + mtx.m[2][2] - mtx.m[0][0] - mtx.m[1][1] );
						q.w = (mtx.m[1][0] - mtx.m[0][1] ) / s;
						q.x = (mtx.m[0][2] + mtx.m[2][0] ) / s;
						q.y = (mtx.m[1][2] + mtx.m[2][1] ) / s;
						q.z = 0.25f * s;
					}
				}

				return q;
			}

			nc::math::Matrix33 matrix33Scale( real_t sx, real_t sy, real_t sz )
			{
				return nc::math::Matrix33(
					sx,	0,	0,
					0,	sy,	0,
					0,	0,	sz 
					);
			}



			nc::math::Matrix44 matrix44RotateAxisX( real_t rad )
			{
				return matrix33RotateAxisX( rad );
			}

			nc::math::Matrix44 matrix44RotateAxisY( real_t rad )
			{
				return matrix33RotateAxisY( rad );
			}

			nc::math::Matrix44 matrix44RotateAxisZ( real_t rad )
			{
				return matrix33RotateAxisZ( rad );
			}

			nc::math::Matrix44 matrix44RotateAxis( const nc::math::Vector4& axis, real_t rad )
			{
				return matrix33RotateAxis( axis, rad );
			}


			nc::math::Matrix44 matrix44Scale( const nc::math::Vector3& sv )
			{
				return matrix44Scale( sv.x, sv.y, sv.z, 1 );
			}

			nc::math::Matrix44 matrix44Scale( const nc::math::Vector4& sv )
			{
				return matrix44Scale( sv.x, sv.y, sv.z, sv.w );
			}

			nc::math::Matrix44 matrix44Scale( real_t sx, real_t sy, real_t sz, real_t sw )
			{
				return nc::math::Matrix44(
					sx,	0,	0,	0,
					0,	sy,	0,	0,
					0,	0,	sz,	0,
					0,	0,	0,	sw
				);
			}

			nc::math::Matrix44 matrix44Translate( const nc::math::Vector3& tv )
			{
				return matrix44Translate( tv.x, tv.y, tv.z, 1 );
			}

			nc::math::Matrix44 matrix44Translate( const nc::math::Vector4& tv )
			{
				return matrix44Translate( tv.x, tv.y, tv.z, tv.w );
			}

			nc::math::Matrix44 matrix44Translate( real_t tx, real_t ty, real_t tz, real_t tw )
			{
				return nc::math::Matrix44(
					1,	0,	0,	tx,
					0,	1,	0,	ty,
					0,	0,	1,	tz,
					0,	0,	0,	tw
				);
			}

			nc::math::Matrix44 matrix44LookAt( const nc::math::Vector3& eye, const nc::math::Vector3& at, const nc::math::Vector3& up )
			{
				/*
				//DirectXの実装(RH系)（nuclの実装は転置）
				zaxis = normal(Eye - At)
				xaxis = normal(cross(Up, zaxis))
				yaxis = cross(zaxis, xaxis)

				xaxis.x           yaxis.x           zaxis.x          0
				xaxis.y           yaxis.y           zaxis.y          0
				xaxis.z           yaxis.z           zaxis.z          0
				-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1
				*/

				nc::math::Vector3 zaxis = nc::math::Vector3(eye - at).normalize();
				nc::math::Vector3 xaxis = nc::math::cross( up, zaxis ).normalize();
				nc::math::Vector3 yaxis = nc::math::cross( zaxis, xaxis );

				return nc::math::Matrix44(
					nc::math::Vector4( xaxis, nc::math::dot( -xaxis, eye ) ),
					nc::math::Vector4( yaxis, nc::math::dot( -yaxis, eye ) ),
					nc::math::Vector4( zaxis, nc::math::dot( -zaxis, eye ) ),
					nc::math::Vector4AxisW
				);
			}


			nc::math::Matrix44 matrix44Ortho( real_t w, real_t h, real_t zn, real_t zf )
			{
				return nc::math::Matrix44(
					2/w,	0,		0,			0,
					0,		2/h,	0,			0,
					0,		0,		1/(zn-zf),	zn/(zn-zf),
					0,		0,		0,			1
				);
			}

			nc::math::Matrix44 matrix44OrthoOffCenter( real_t l, real_t r, real_t b, real_t t, real_t zn, real_t zf )
			{
				return nc::math::Matrix44(
					2/(r-l),	0,			0,			(l+r)/(l-r),
					0,			2/(t-b),	0,			(t+b)/(b-t),
					0,			0,			1/(zn-zf),	zn/(zn-zf),
					0,			0,			0,			1
				);
			}


			nc::math::Matrix44 matrix44Perspective( real_t w, real_t h, real_t zn, real_t zf )
			{
				return nc::math::Matrix44(
					2*zn/w,	0,		0,				0,
					0,		2*zn/h,	0,				0,
					0,		0,		zf/(zn-zf),		zn*zf/(zn-zf),
					0,		0,		-1,				0
				);
			}

			nc::math::Matrix44 matrix44PerspectiveOffCenter(  real_t l, real_t r, real_t b, real_t t, real_t zn, real_t zf )
			{
				return nc::math::Matrix44(
					2*zn/(r-l),	0,			(l+r)/(r-l),	0,
					0,			2*zn/(t-b),	(t+b)/(t-b),	0,
					0,			0,			zf/(zn-zf),		zn*zf/(zn-zf),
					0,			0,			-1,				0
				);
			}

			nc::math::Matrix44 matrix44PerspectiveFov( real_t fovY, real_t aspect, real_t zn, real_t zf )
			{
				real_t ys = 1 / tan(fovY/2); //cotangent(fovY/2)
				real_t xs = ys / aspect;

				return nc::math::Matrix44(
					xs,		0,		0,				0,
					0,		ys,		0,				0,
					0,		0,		zf/(zn-zf),		zn*zf/(zn-zf),
					0,		0,		-1,				0
					);
			}


			nc::math::Vector2 transform( const nc::math::Matrix33& mtx, const nc::math::Vector2& vec )
			{
				nc::math::Vector3 vec3( vec, 0 );

				return nc::math::Vector3(
					nc::math::dot( mtx.x, vec3 ),
					nc::math::dot( mtx.y, vec3 ),
					nc::math::dot( mtx.z, vec3 )
				).orthoProjection();
			}

			nc::math::Vector3 transform( const nc::math::Matrix33& mtx, const nc::math::Vector3& vec )
			{
				return nc::math::Vector3(
					nc::math::dot( mtx.x, vec ),
					nc::math::dot( mtx.y, vec ),
					nc::math::dot( mtx.z, vec )
				);
			}


			nc::math::Vector3 transform( const nc::math::Matrix44& mtx, const nc::math::Vector3& vec )
			{
				//平行移動が行われない
				nc::math::Matrix33 mtx33( mtx );
				return nc::math::transform( mtx33, vec );
			}

			nc::math::Vector4 transform( const nc::math::Matrix44& mtx, const nc::math::Vector4& vec )
			{
				return nc::math::Vector4(
					nc::math::dot( mtx.x, vec ),
					nc::math::dot( mtx.y, vec ),
					nc::math::dot( mtx.z, vec ),
					nc::math::dot( mtx.w, vec )
				);
			}

			nc::math::Point2 transform( const nc::math::Matrix33& mtx, const nc::math::Point2& pt )
			{
				return nc::math::transform( mtx, nc::math::Vector3(pt, 1) ).orthoProjection();
			}

			nc::math::Point3 transform( const nc::math::Matrix33& mtx, const nc::math::Point3& pt )
			{
				return nc::math::transform( mtx, nc::math::Vector3(pt) );
			}


			nc::math::Point3 transform( const nc::math::Matrix44& mtx, const nc::math::Point3& pt )
			{
				//wを1として計算する
				return nc::math::transform( mtx, nc::math::Point4( pt, 1 ) ).orthoProjection();
			}

			nc::math::Point4 transform( const nc::math::Matrix44& mtx, const nc::math::Point4& pt )
			{
				return nc::math::transform( mtx, static_cast<nc::math::Vector4>(pt) );
			}
			
		}//namespace math
	
	}//namespace generic

}//namespace nc