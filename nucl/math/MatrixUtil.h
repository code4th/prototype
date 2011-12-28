#ifndef __NC_MATRIXUTIL_h__
#define __NC_MATRIXUTIL_h__

#include "Matrix.h"

namespace nc{

	//-------------------------------------------------

	namespace generic{

		namespace math{

			//Matrixの形はOpenGL形式となり
			//左から行列を掛けていく
		
			//平行移動行列
			//	1,	0,	0,	tx,
			//	0,	1,	0,	ty,
			//	0,	0,	1,	tz,
			//	0,	0,	0,	1

			//DirectX -> World * Camera * Screnn
			//OpenGL -> Screen * Camera * World

			//DirectX形式とOpenGL形式は、転置の関係にあるので
			//相互変換は転置行列を求める事で変換可能である。
			//ただし、掛け算の方向が変わるので注意する事。

			//座標系は基本的に右手系を採用しています。
		

			nc::math::Matrix33 matrix33RotateAxisX( real_t rad );
			nc::math::Matrix33 matrix33RotateAxisY( real_t rad );
			nc::math::Matrix33 matrix33RotateAxisZ( real_t rad );

			//axisは単位ベクトルである必要がある。
			nc::math::Matrix33 matrix33RotateAxis( const nc::math::Vector4& axis, real_t rad );

			/*
			Yaw
			y 軸のヨー (ラジアン単位)
			Pitch
			x 軸のピッチ (ラジアン単位)
			Roll
			z 軸のロール (ラジアン単位)
			*/
			/*
			トランスフォームは、ロール、ピッチ、ヨーの順に行われます。
			オブジェクトのローカル座標軸を基準にした場合、このトランスフォーム順序は
			z 軸、x 軸、y 軸をこの順に回転軸として使用した回転に等しくなります。
			*/
			nc::math::Matrix33 matrix33RotateYawPitchRoll( real_t yaw, real_t pitch, real_t roll );

			nc::math::Matrix33 matrix33RotateQuaternion( const Quaternion& q );

			nc::math::Quaternion quaternionRotateMatrix( const nc::math::Matrix33& mtx );

			nc::math::Matrix33 matrix33Scale( real_t sx, real_t sy, real_t sz );

			nc::math::Matrix44 matrix44RotateAxisX( real_t rad );
			nc::math::Matrix44 matrix44RotateAxisY( real_t rad );
			nc::math::Matrix44 matrix44RotateAxisZ( real_t rad );
			nc::math::Matrix44 matrix44RotateAxis( const nc::math::Vector4& axis, real_t rad );

			nc::math::Matrix44 matrix44Scale( const nc::math::Vector3& sv );
			nc::math::Matrix44 matrix44Scale( const nc::math::Vector4& sv );
			nc::math::Matrix44 matrix44Scale( real_t sx, real_t sy, real_t sz, real_t sw = 1 );

			nc::math::Matrix44 matrix44Translate( const nc::math::Vector3& tv );
			nc::math::Matrix44 matrix44Translate( const nc::math::Vector4& tv );
			nc::math::Matrix44 matrix44Translate( real_t tx, real_t ty, real_t tz, real_t tw = 1 );

			/*
			eye
			視点
			at
			カメラの注視対象
			up
			現在のワールド座標における上方向。この構造体の値は通常 [0, 1, 0] です。
			*/
			nc::math::Matrix44 matrix44LookAt( const Vector3& eye, const Vector3& at, const Vector3& up );
	
			/*
			w
			[in] ビュー ボリュームの幅。 
			h
			[in] ビュー ボリュームの高さ。 
			zn
			[in] ビュー ボリュームの最小 z 値。 
			zf
			[in] ビュー ボリュームの最大 z 値。
			*/
			nc::math::Matrix44 matrix44Ortho( real_t w, real_t h, real_t zn, real_t zf );

			/*
			l
			ビュー ボリュームの最小 x 値
			r
			ビュー ボリュームの最大 x 値
			b
			ビュー ボリュームの最小 y 値
			t
			ビュー ボリュームの最大 y 値
			zn
			ビュー ボリュームの最小 z 値
			zf
			ビュー ボリュームの最大 z 値
			*/
			nc::math::Matrix44 matrix44OrthoOffCenter( real_t l, real_t r, real_t b, real_t t, real_t zn, real_t zf );

			/*
			w
			近くのビュー プレーンでのビュー ボリュームの幅
			h
			近くのビュー プレーンでのビュー ボリュームの高さ
			zn
			近くのビュー プレーンの z 値
			zf
			遠くのビュー プレーンの z 値
			*/
			nc::math::Matrix44 matrix44Perspective( real_t w, real_t h, real_t zn, real_t zf );

			/*
			l
			ビュー ボリュームの最小 x 値
			r
			ビュー ボリュームの最大 x 値
			b
			ビュー ボリュームの最小 y 値
			t
			ビュー ボリュームの最大 y 値
			zn
			ビュー ボリュームの最小 z 値
			zf
			ビュー ボリュームの最大 z 値
			*/
			nc::math::Matrix44 matrix44PerspectiveOffCenter(  real_t l, real_t r, real_t b, real_t t, real_t zn, real_t zf );

			/*
			fovy
			y 方向の視野角 (ラジアン単位)
			Aspect
			アスペクト比 (ビュー空間の幅を高さで除算して定義)
			zn
			近くのビュー プレーンの z 値
			zf
			遠くのビュー プレーンの z 値
			*/
			nc::math::Matrix44 matrix44PerspectiveFov( real_t fovY, real_t aspect, real_t zn, real_t zf );


			//Matrixの計算の際に、どっちにしろ一時変数に計算途中を格納するので
			//あえて参照で受け取って、そのものを変更するようにはしていません。
			
			nc::math::Vector2 transform( const nc::math::Matrix33& mtx, const nc::math::Vector2& vec );	//Z = 0として計算されます。
			nc::math::Vector3 transform( const nc::math::Matrix33& mtx, const nc::math::Vector3& vec );

			nc::math::Vector3 transform( const nc::math::Matrix44& mtx, const nc::math::Vector3& vec );	//W = 0として計算されます。
			nc::math::Vector4 transform( const nc::math::Matrix44& mtx, const nc::math::Vector4& vec );

			nc::math::Point2 transform( const nc::math::Matrix33& mtx, const nc::math::Point2& pt );		//Z = 1として計算されます。
			nc::math::Point3 transform( const nc::math::Matrix33& mtx, const nc::math::Point3& pt );

			nc::math::Point3 transform( const nc::math::Matrix44& mtx, const nc::math::Point3& pt );		//W = 1として計算されます。
			nc::math::Point4 transform( const nc::math::Matrix44& mtx, const nc::math::Point4& pt );
			
		}//namespace math

	}//namespace generic

}//namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MATRIX_UTIL

#include "PLT_MatrixUtil.h"

#else

namespace nc{

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
		using nc::generic::math::transform;
		
	}//namespace math

}//namespace nc

#endif


#endif // __NC_MATRIXUTIL_h__