#ifndef __NC_MATRIX_h__
#define __NC_MATRIX_h__

#include "Coordinate.h"
#include "Vector.h"
#include "VectorUtil.h"
#include "Quaternion.h"
#include "Point.h"
#include "MatrixData.h"

namespace nc{
	
	namespace generic {

		namespace math{

			template< typename MatrixDataT >
			class IMatrix33;

			template< typename MatrixDataT >
			class IMatrix44;

			template< typename MatrixDataT >
			class IMatrix33 : public MatrixDataT{
			public:
				typedef	MatrixDataT		MatrixData_t;

				IMatrix33();
				IMatrix33( const IMatrix33& rhs );
				IMatrix33( const IMatrix44< MatrixDataT >& rhs );
				IMatrix33( const Matrix33RowMajorData& rhs );
				IMatrix33( const Matrix33ColMajorData& rhs );
				IMatrix33( const Matrix44RowMajorData& rhs );
				IMatrix33( const Matrix44ColMajorData& rhs );

				IMatrix33( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature = MatrixDataT::MAJOR_TYPE_SIGNATURE );
				IMatrix33( const Coordinate3 arr[], int importMajorTypeSignature = MatrixDataT::MAJOR_TYPE_SIGNATURE );
				IMatrix33(	real_t m11, real_t m12, real_t m13,
							real_t m21, real_t m22, real_t m23,
							real_t m31, real_t m32, real_t m33 );
				IMatrix33( const real_t arr[], int importMajorTypeSignature = MatrixDataT::MAJOR_TYPE_SIGNATURE );

				template< typename rhsT >
				const IMatrix33& operator= ( const rhsT& rhs ){ return static_cast< IMatrix33& >( assign(rhs) ); }

				//íPçÄ +/-ââéZéq
				IMatrix33 operator+ () const;
				IMatrix33 operator- () const;

				IMatrix33 operator* ( const IMatrix33& rhs ) const;
				IMatrix33 operator+ ( const IMatrix33& rhs ) const;
				IMatrix33 operator- ( const IMatrix33& rhs ) const;
				const IMatrix33& operator*=( const IMatrix33& rhs );
				const IMatrix33& operator+=( const IMatrix33& rhs );
				const IMatrix33& operator-=( const IMatrix33& rhs );

				IMatrix33 operator* ( real_t bias ) const;
				IMatrix33 operator/ ( real_t bias ) const;
				const IMatrix33& operator*=( real_t bias );
				const IMatrix33& operator/=( real_t bias );

				//î‰ärââéZ
				bool operator== ( const IMatrix33& rhs ) const;
				bool operator!= ( const IMatrix33& rhs ) const;

				//çsóÒéÆÇÃâÇï‘ÇµÇ‹Ç∑
				real_t determinant() const;

				//ãtçsóÒÇï‘ÇµÇ‹Ç∑
				IMatrix33 inverse() const;

				//ç∂éË/âEéËç¿ïWånÇêÿÇËë÷Ç¶Ç‹Ç∑ÅB
				IMatrix33 changeHandside() const;

			};

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > operator * ( real_t lhs, const IMatrix33< MatrixDataT >& rhs );


			template< typename MatrixDataT >
			class IMatrix44 : public MatrixDataT{
			public:
				typedef	MatrixDataT		MatrixData_t;

				IMatrix44();
				IMatrix44( const IMatrix44& rhs );
				IMatrix44( const IMatrix33< MatrixDataT >& rhs );
				IMatrix44( const Matrix33RowMajorData& rhs );
				IMatrix44( const Matrix33ColMajorData& rhs );
				IMatrix44( const Matrix44RowMajorData& rhs );
				IMatrix44( const Matrix44ColMajorData& rhs );

				IMatrix44( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature = MatrixDataT::MAJOR_TYPE_SIGNATURE );
				IMatrix44( const Coordinate4 arr[], int importMajorTypeSignature = MatrixDataT::MAJOR_TYPE_SIGNATURE );
				IMatrix44(	real_t m11, real_t m12, real_t m13, real_t m14,
							real_t m21, real_t m22, real_t m23, real_t m24,
							real_t m31, real_t m32, real_t m33, real_t m34,
							real_t m41, real_t m42, real_t m43, real_t m44 );
				IMatrix44( const real_t arr[], int importMajorTypeSignature = MatrixDataT::MAJOR_TYPE_SIGNATURE );

				template< typename rhsT >
				const IMatrix44& operator= ( const rhsT& rhs ){ return static_cast< IMatrix44& >( assign(rhs) ); }

				//íPçÄ +/-ââéZéq
				IMatrix44 operator+ () const;
				IMatrix44 operator- () const;

				//â¡å∏éZÇ∆ä|ÇØéZ
				IMatrix44 operator* ( const IMatrix44& rhs ) const;
				IMatrix44 operator+ ( const IMatrix44& rhs ) const;
				IMatrix44 operator- ( const IMatrix44& rhs ) const;
				const IMatrix44& operator*=( const IMatrix44& rhs );
				const IMatrix44& operator+=( const IMatrix44& rhs );
				const IMatrix44& operator-=( const IMatrix44& rhs );

				//ÉXÉJÉâÅ[î{
				IMatrix44 operator* ( real_t bias ) const;
				IMatrix44 operator/ ( real_t bias ) const;
				const IMatrix44& operator*=( real_t bias );
				const IMatrix44& operator/=( real_t bias );


				//î‰ärââéZ
				bool operator== ( const IMatrix44& rhs ) const;
				bool operator!= ( const IMatrix44& rhs ) const;

		
				//çsóÒéÆÇÃâÇï‘ÇµÇ‹Ç∑
				real_t determinant() const;

				//ãtçsóÒÇï‘ÇµÇ‹Ç∑
				IMatrix44 inverse() const;

				//ç∂éË/âEéËç¿ïWånÇêÿÇËë÷Ç¶Ç‹Ç∑ÅB
				IMatrix44 changeHandside() const;
			};

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > operator * ( real_t lhs, const IMatrix44< MatrixDataT >& rhs );

			

			typedef IMatrix33< nc::math::Matrix33Data >	Matrix33;
			typedef IMatrix44< nc::math::Matrix44Data >	Matrix44;



			//------------------------------------------------------------------


			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33()
			{

			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const IMatrix33& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const IMatrix44< MatrixDataT >& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const Matrix33RowMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const Matrix33ColMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const Matrix44RowMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const Matrix44ColMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature )
			{
				MatrixData_t::assign( x, y, z, importMajorTypeSignature );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const Coordinate3 arr[], int importMajorTypeSignature )
			{
				MatrixData_t::assign( arr, importMajorTypeSignature );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( real_t m11, real_t m12, real_t m13, real_t m21, real_t m22, real_t m23, real_t m31, real_t m32, real_t m33 )
			{
				MatrixData_t::assign( m11, m12, m13, 
						m21, m22, m23,
						m31, m32, m33 );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT >::IMatrix33( const real_t arr[], int importMajorTypeSignature )
			{
				MatrixData_t::assign( arr, importMajorTypeSignature );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator+() const
			{
				return *this;
			}


			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator-() const
			{
				IMatrix33< MatrixDataT > tmp;
				for( int n = 0; n < 9; n++ )
				{
					tmp.idx[n] = -idx[n];
				}
				return tmp;
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator*( const IMatrix33< MatrixDataT >& rhs ) const
			{
				Vector3 rowVec[3] = { MatrixData_t::rowVector(0), MatrixData_t::rowVector(1), MatrixData_t::rowVector(2) };

				return IMatrix33< MatrixDataT >(
					nc::math::dot( rowVec[0], rhs.colVector(0) ),
					nc::math::dot( rowVec[0], rhs.colVector(1) ),
					nc::math::dot( rowVec[0], rhs.colVector(2) ),

					nc::math::dot( rowVec[1], rhs.colVector(0) ),
					nc::math::dot( rowVec[1], rhs.colVector(1) ),
					nc::math::dot( rowVec[1], rhs.colVector(2) ),

					nc::math::dot( rowVec[2], rhs.colVector(0) ),
					nc::math::dot( rowVec[2], rhs.colVector(1) ),
					nc::math::dot( rowVec[2], rhs.colVector(2) )
					);
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator+( const IMatrix33< MatrixDataT >& rhs ) const
			{
				return IMatrix33< MatrixDataT >(
					idx[0] + rhs.idx[0],
					idx[1] + rhs.idx[1],
					idx[2] + rhs.idx[2],
					idx[3] + rhs.idx[3],
					idx[4] + rhs.idx[4],
					idx[5] + rhs.idx[5],
					idx[6] + rhs.idx[6],
					idx[7] + rhs.idx[7],
					idx[8] + rhs.idx[8]
				);
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator-( const IMatrix33< MatrixDataT >& rhs ) const
			{
				return IMatrix33< MatrixDataT >(
					idx[0] - rhs.idx[0],
					idx[1] - rhs.idx[1],
					idx[2] - rhs.idx[2],
					idx[3] - rhs.idx[3],
					idx[4] - rhs.idx[4],
					idx[5] - rhs.idx[5],
					idx[6] - rhs.idx[6],
					idx[7] - rhs.idx[7],
					idx[8] - rhs.idx[8]
				);
			}

			template< typename MatrixDataT >
			const IMatrix33< MatrixDataT >& IMatrix33< MatrixDataT >::operator*=( const IMatrix33< MatrixDataT >& rhs )
			{
				return static_cast< IMatrix33< MatrixDataT >& >( MatrixData_t::assign( *this * rhs ) );
			}

			template< typename MatrixDataT >
			const IMatrix33< MatrixDataT >& IMatrix33< MatrixDataT >::operator+=( const IMatrix33< MatrixDataT >& rhs )
			{
				return static_cast< IMatrix33< MatrixDataT >& >( MatrixData_t::assign( *this + rhs ) );
			}

			template< typename MatrixDataT >
			const IMatrix33< MatrixDataT >& IMatrix33< MatrixDataT >::operator-=( const IMatrix33< MatrixDataT >& rhs )
			{
				return static_cast< IMatrix33< MatrixDataT >& >( MatrixData_t::assign( *this - rhs ) );
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator*( real_t bias ) const
			{
				IMatrix33< MatrixDataT > tmp;
				for( int n = 0; n < 9; n++ )
				{
					tmp.idx[n] = idx[n] * bias;
				}
				return tmp;
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::operator/( real_t bias ) const
			{
				IMatrix33< MatrixDataT > tmp;
				for( int n = 0; n < 9; n++ )
				{
					tmp.idx[n] = idx[n] / bias;
				}
				return tmp;
			}

			template< typename MatrixDataT >
			const IMatrix33< MatrixDataT >& IMatrix33< MatrixDataT >::operator*=( real_t bias )
			{
				return static_cast< IMatrix33< MatrixDataT >& >( MatrixData_t::assign( *this * bias ) );
			}

			template< typename MatrixDataT >
			const IMatrix33< MatrixDataT >& IMatrix33< MatrixDataT >::operator/=( real_t bias )
			{
				return static_cast< IMatrix33< MatrixDataT >& >( MatrixData_t::assign( *this / bias ) );
			}

			template< typename MatrixDataT >
			bool IMatrix33< MatrixDataT >::operator==( const IMatrix33< MatrixDataT >& rhs ) const
			{
				return x == rhs.x && y == rhs.y && z == rhs.z;
			}

			template< typename MatrixDataT >
			bool IMatrix33< MatrixDataT >::operator!=( const IMatrix33< MatrixDataT >& rhs ) const
			{
				return !operator==(rhs);
			}

			template< typename MatrixDataT >
			real_t IMatrix33< MatrixDataT >::determinant() const
			{
				//çsóÒéÆÇÃãÅÇﬂï˚
				//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

				return m11 * m22 * m33 + m21 * m32 * m13 + m31 * m12 * m23 - m11 * m32 * m23 - m31 * m22 * m13 - m21 * m12 * m33;
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::inverse() const
			{
				//ãtçsóÒÇÃãÅÇﬂï˚
				//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

				real_t det = determinant();
				if( isEqual( det, 0 ) ){
					return nc::math::Matrix33Unit;
				}

				IMatrix33< MatrixDataT > tmp;

				tmp.m11 = m22 * m33 - m23 * m32;
				tmp.m12 = m13 * m32 - m12 * m33;
				tmp.m13 = m12 * m23 - m13 * m22;

				tmp.m21 = m23 * m31 - m21 * m33;
				tmp.m22 = m11 * m33 - m13 * m31;
				tmp.m23 = m13 * m21 - m11 * m23;

				tmp.m31 = m21 * m32 - m22 * m31;
				tmp.m32 = m12 * m31 - m11 * m32;
				tmp.m33 = m11 * m22 - m12 * m21;

				return tmp / det;
			}

			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > IMatrix33< MatrixDataT >::changeHandside() const
			{
				return IMatrix33< MatrixDataT >(
					x, y, -z
					);
			}


			template< typename MatrixDataT >
			IMatrix33< MatrixDataT > operator * ( real_t lhs, const IMatrix33< MatrixDataT >& rhs )
			{
				return IMatrix33< MatrixDataT >( rhs * lhs );
			}

			//---------------------------------------------------------

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44()
			{

			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const IMatrix44& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const IMatrix33< MatrixDataT >& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const Matrix33RowMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const Matrix33ColMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const Matrix44RowMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const Matrix44ColMajorData& rhs )
			{
				MatrixData_t::assign( rhs );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature )
			{
				MatrixData_t::assign( x, y, z, w, importMajorTypeSignature );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const Coordinate4 arr[], int importMajorTypeSignature )
			{
				MatrixData_t::assign( arr, importMajorTypeSignature );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44(	real_t m11, real_t m12, real_t m13, real_t m14,
				real_t m21, real_t m22, real_t m23, real_t m24,
				real_t m31, real_t m32, real_t m33, real_t m34,
				real_t m41, real_t m42, real_t m43, real_t m44 )
			{
				MatrixData_t::assign( m11, m12, m13, m14, 
						m21, m22, m23, m24,
						m31, m32, m33, m34,
						m41, m42, m43, m44 );
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT >::IMatrix44( const real_t arr[], int importMajorTypeSignature )
			{
				MatrixData_t::assign( arr, importMajorTypeSignature );
			}


			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator+() const
			{
				return *this;
			}


			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator-() const
			{
				IMatrix44< MatrixDataT > tmp;
				for( int n = 0; n < 16; n++ )
				{
					tmp.idx[n] = -idx[n];
				}
				return tmp;
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator*( const IMatrix44< MatrixDataT >& rhs ) const
			{
				Coordinate4Data rowVec[4] = { rowVector(0), rowVector(1), rowVector(2), rowVector(3) };

				return IMatrix44< MatrixDataT >(
					nc::math::dot( rowVec[0], rhs.colVector(0) ),
					nc::math::dot( rowVec[0], rhs.colVector(1) ),
					nc::math::dot( rowVec[0], rhs.colVector(2) ),
					nc::math::dot( rowVec[0], rhs.colVector(3) ),

					nc::math::dot( rowVec[1], rhs.colVector(0) ),
					nc::math::dot( rowVec[1], rhs.colVector(1) ),
					nc::math::dot( rowVec[1], rhs.colVector(2) ),
					nc::math::dot( rowVec[1], rhs.colVector(3) ),

					nc::math::dot( rowVec[2], rhs.colVector(0) ),
					nc::math::dot( rowVec[2], rhs.colVector(1) ),
					nc::math::dot( rowVec[2], rhs.colVector(2) ),
					nc::math::dot( rowVec[2], rhs.colVector(3) ),

					nc::math::dot( rowVec[3], rhs.colVector(0) ),
					nc::math::dot( rowVec[3], rhs.colVector(1) ),
					nc::math::dot( rowVec[3], rhs.colVector(2) ),
					nc::math::dot( rowVec[3], rhs.colVector(3) )
					);
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator+( const IMatrix44< MatrixDataT >& rhs ) const
			{
				return IMatrix44< MatrixDataT >(
					idx[0] + rhs.idx[0],
					idx[1] + rhs.idx[1],
					idx[2] + rhs.idx[2],
					idx[3] + rhs.idx[3],
					idx[4] + rhs.idx[4],
					idx[5] + rhs.idx[5],
					idx[6] + rhs.idx[6],
					idx[7] + rhs.idx[7],
					idx[8] + rhs.idx[8],
					idx[9] + rhs.idx[9],
					idx[10] + rhs.idx[10],
					idx[11] + rhs.idx[11],
					idx[12] + rhs.idx[12],
					idx[13] + rhs.idx[13],
					idx[14] + rhs.idx[14],
					idx[15] + rhs.idx[15]
				);
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator-( const IMatrix44< MatrixDataT >& rhs ) const
			{
				return IMatrix44< MatrixDataT >(
					idx[0] - rhs.idx[0],
					idx[1] - rhs.idx[1],
					idx[2] - rhs.idx[2],
					idx[3] - rhs.idx[3],
					idx[4] - rhs.idx[4],
					idx[5] - rhs.idx[5],
					idx[6] - rhs.idx[6],
					idx[7] - rhs.idx[7],
					idx[8] - rhs.idx[8],
					idx[9] - rhs.idx[9],
					idx[10] - rhs.idx[10],
					idx[11] - rhs.idx[11],
					idx[12] - rhs.idx[12],
					idx[13] - rhs.idx[13],
					idx[14] - rhs.idx[14],
					idx[15] - rhs.idx[15]
				);
			}

			template< typename MatrixDataT >
			const IMatrix44< MatrixDataT >& IMatrix44< MatrixDataT >::operator*=( const IMatrix44< MatrixDataT >& rhs )
			{
				return static_cast< IMatrix44< MatrixDataT >& >( MatrixData_t::assign( *this * rhs ) );
			}

			template< typename MatrixDataT >
			const IMatrix44< MatrixDataT >& IMatrix44< MatrixDataT >::operator+=( const IMatrix44< MatrixDataT >& rhs )
			{
				return static_cast< IMatrix44< MatrixDataT >& >( MatrixData_t::assign( *this + rhs ) ); 
			}

			template< typename MatrixDataT >
			const IMatrix44< MatrixDataT >& IMatrix44< MatrixDataT >::operator-=( const IMatrix44< MatrixDataT >& rhs )
			{
				return static_cast< IMatrix44< MatrixDataT >& >( MatrixData_t::assign( *this - rhs ) );
			}


			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator*( real_t bias ) const
			{
				IMatrix44< MatrixDataT > tmp;
				for( int n = 0; n < 16; n++ )
				{
					tmp.idx[n] = MatrixDataT::idx[n] * bias;
				}
				return tmp;
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::operator/( real_t bias ) const
			{
				IMatrix44< MatrixDataT > tmp;
				for( int n = 0; n < 16; n++ )
				{
					tmp.idx[n] = MatrixDataT::idx[n] / bias;
				}
				return tmp;
			}

			template< typename MatrixDataT >
			const IMatrix44< MatrixDataT >& IMatrix44< MatrixDataT >::operator*=( real_t bias )
			{
				return static_cast< IMatrix44< MatrixDataT >& >( MatrixData_t::assign( *this * bias ) );
			}

			template< typename MatrixDataT >
			const IMatrix44< MatrixDataT >& IMatrix44< MatrixDataT >::operator/=( real_t bias )
			{
				return static_cast< IMatrix44< MatrixDataT >& >( MatrixData_t::assign( *this / bias ) );
			}

			template< typename MatrixDataT >
			bool IMatrix44< MatrixDataT >::operator==( const IMatrix44< MatrixDataT >& rhs ) const
			{
				return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
			}

			template< typename MatrixDataT >
			bool IMatrix44< MatrixDataT >::operator!=( const IMatrix44< MatrixDataT >& rhs ) const
			{
				return !operator==(rhs);
			}

			template< typename MatrixDataT >
			real_t IMatrix44< MatrixDataT >::determinant() const
			{
				//çsóÒéÆÇÃãÅÇﬂï˚
				//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

				return	MatrixDataT::m11 * MatrixDataT::m22 * MatrixDataT::m33 * MatrixDataT::m44 + MatrixDataT::m11 * MatrixDataT::m23 * MatrixDataT::m34 * MatrixDataT::m42 + MatrixDataT::m11 * MatrixDataT::m24 * MatrixDataT::m32 * MatrixDataT::m43
					+ MatrixDataT::m12 * MatrixDataT::m21 * MatrixDataT::m34 * MatrixDataT::m43 + MatrixDataT::m12 * MatrixDataT::m23 * MatrixDataT::m31 * MatrixDataT::m44 + MatrixDataT::m12 * MatrixDataT::m24 * MatrixDataT::m33 * MatrixDataT::m41
					+ MatrixDataT::m13 * MatrixDataT::m21 * MatrixDataT::m32 * MatrixDataT::m44 + MatrixDataT::m13 * MatrixDataT::m22 * MatrixDataT::m34 * MatrixDataT::m41 + MatrixDataT::m13 * MatrixDataT::m24 * MatrixDataT::m31 * MatrixDataT::m42
					+ MatrixDataT::m14 * MatrixDataT::m21 * MatrixDataT::m33 * MatrixDataT::m42 + MatrixDataT::m14 * MatrixDataT::m22 * MatrixDataT::m31 * MatrixDataT::m43 + MatrixDataT::m14 * MatrixDataT::m23 * MatrixDataT::m32 * MatrixDataT::m41
					- MatrixDataT::m11 * MatrixDataT::m22 * MatrixDataT::m34 * MatrixDataT::m43 - MatrixDataT::m11 * MatrixDataT::m23 * MatrixDataT::m32 * MatrixDataT::m44 - MatrixDataT::m11 * MatrixDataT::m24 * MatrixDataT::m33 * MatrixDataT::m42
					- MatrixDataT::m12 * MatrixDataT::m21 * MatrixDataT::m33 * MatrixDataT::m44 - MatrixDataT::m12 * MatrixDataT::m23 * MatrixDataT::m34 * MatrixDataT::m41 - MatrixDataT::m12 * MatrixDataT::m24 * MatrixDataT::m31 * MatrixDataT::m43
					- MatrixDataT::m13 * MatrixDataT::m21 * MatrixDataT::m34 * MatrixDataT::m42 - MatrixDataT::m13 * MatrixDataT::m22 * MatrixDataT::m31 * MatrixDataT::m44 - MatrixDataT::m13 * MatrixDataT::m24 * MatrixDataT::m32 * MatrixDataT::m41
					- MatrixDataT::m14 * MatrixDataT::m21 * MatrixDataT::m32 * MatrixDataT::m43 - MatrixDataT::m14 * MatrixDataT::m22 * MatrixDataT::m33 * MatrixDataT::m41 - MatrixDataT::m14 * MatrixDataT::m23 * MatrixDataT::m31 * MatrixDataT::m42;
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::inverse() const
			{
				//ãtçsóÒÇÃãÅÇﬂï˚
				//http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/tech23.html

				real_t det = determinant();
				if( isEqual( det, 0 ) ){
					return IMatrix44< MatrixDataT >(	1,0,0,0,
														0,1,0,0,
														0,0,1,0,
														0,0,0,1 );
				}

				IMatrix44< MatrixDataT > tmp;

				tmp.m11 = MatrixDataT::m22 * MatrixDataT::m33 * MatrixDataT::m44 + MatrixDataT::m23 * MatrixDataT::m34 * MatrixDataT::m42 + MatrixDataT::m24 * MatrixDataT::m32 * MatrixDataT::m43 - MatrixDataT::m22 * MatrixDataT::m34 * MatrixDataT::m43 - MatrixDataT::m23 * MatrixDataT::m32 * MatrixDataT::m44 - MatrixDataT::m24 * MatrixDataT::m33 * MatrixDataT::m42;
				tmp.m12 = MatrixDataT::m12 * MatrixDataT::m34 * MatrixDataT::m43 + MatrixDataT::m13 * MatrixDataT::m32 * MatrixDataT::m44 + MatrixDataT::m14 * MatrixDataT::m33 * MatrixDataT::m42 - MatrixDataT::m12 * MatrixDataT::m33 * MatrixDataT::m44 - MatrixDataT::m13 * MatrixDataT::m34 * MatrixDataT::m42 - MatrixDataT::m14 * MatrixDataT::m32 * MatrixDataT::m43;
				tmp.m13 = MatrixDataT::m12 * MatrixDataT::m23 * MatrixDataT::m44 + MatrixDataT::m13 * MatrixDataT::m24 * MatrixDataT::m42 + MatrixDataT::m14 * MatrixDataT::m22 * MatrixDataT::m43 - MatrixDataT::m12 * MatrixDataT::m24 * MatrixDataT::m43 - MatrixDataT::m13 * MatrixDataT::m22 * MatrixDataT::m44 - MatrixDataT::m14 * MatrixDataT::m23 * MatrixDataT::m42;
				tmp.m14 = MatrixDataT::m12 * MatrixDataT::m24 * MatrixDataT::m33 + MatrixDataT::m13 * MatrixDataT::m22 * MatrixDataT::m34 + MatrixDataT::m14 * MatrixDataT::m23 * MatrixDataT::m32 - MatrixDataT::m12 * MatrixDataT::m23 * MatrixDataT::m34 - MatrixDataT::m13 * MatrixDataT::m24 * MatrixDataT::m32 - MatrixDataT::m14 * MatrixDataT::m22 * MatrixDataT::m33;

				tmp.m21 = MatrixDataT::m21 * MatrixDataT::m34 * MatrixDataT::m43 + MatrixDataT::m23 * MatrixDataT::m31 * MatrixDataT::m44 + MatrixDataT::m24 * MatrixDataT::m33 * MatrixDataT::m41 - MatrixDataT::m21 * MatrixDataT::m33 * MatrixDataT::m44 - MatrixDataT::m23 * MatrixDataT::m34 * MatrixDataT::m41 - MatrixDataT::m24 * MatrixDataT::m31 * MatrixDataT::m43;
				tmp.m22 = MatrixDataT::m11 * MatrixDataT::m33 * MatrixDataT::m44 + MatrixDataT::m13 * MatrixDataT::m34 * MatrixDataT::m41 + MatrixDataT::m14 * MatrixDataT::m31 * MatrixDataT::m43 - MatrixDataT::m11 * MatrixDataT::m34 * MatrixDataT::m43 - MatrixDataT::m13 * MatrixDataT::m31 * MatrixDataT::m44 - MatrixDataT::m14 * MatrixDataT::m33 * MatrixDataT::m41;
				tmp.m23 = MatrixDataT::m11 * MatrixDataT::m24 * MatrixDataT::m43 + MatrixDataT::m13 * MatrixDataT::m21 * MatrixDataT::m44 + MatrixDataT::m14 * MatrixDataT::m23 * MatrixDataT::m41 - MatrixDataT::m11 * MatrixDataT::m23 * MatrixDataT::m44 - MatrixDataT::m13 * MatrixDataT::m24 * MatrixDataT::m41 - MatrixDataT::m14 * MatrixDataT::m21 * MatrixDataT::m43;
				tmp.m24 = MatrixDataT::m11 * MatrixDataT::m23 * MatrixDataT::m34 + MatrixDataT::m13 * MatrixDataT::m24 * MatrixDataT::m31 + MatrixDataT::m14 * MatrixDataT::m21 * MatrixDataT::m33 - MatrixDataT::m11 * MatrixDataT::m24 * MatrixDataT::m33 - MatrixDataT::m13 * MatrixDataT::m21 * MatrixDataT::m34 - MatrixDataT::m14 * MatrixDataT::m23 * MatrixDataT::m31;

				tmp.m31 = MatrixDataT::m21 * MatrixDataT::m32 * MatrixDataT::m44 + MatrixDataT::m22 * MatrixDataT::m34 * MatrixDataT::m41 + MatrixDataT::m24 * MatrixDataT::m31 * MatrixDataT::m42 - MatrixDataT::m21 * MatrixDataT::m34 * MatrixDataT::m42 - MatrixDataT::m22 * MatrixDataT::m31 * MatrixDataT::m44 - MatrixDataT::m24 * MatrixDataT::m32 * MatrixDataT::m41;
				tmp.m32 = MatrixDataT::m11 * MatrixDataT::m34 * MatrixDataT::m42 + MatrixDataT::m12 * MatrixDataT::m31 * MatrixDataT::m44 + MatrixDataT::m14 * MatrixDataT::m32 * MatrixDataT::m41 - MatrixDataT::m11 * MatrixDataT::m32 * MatrixDataT::m44 - MatrixDataT::m12 * MatrixDataT::m34 * MatrixDataT::m41 - MatrixDataT::m14 * MatrixDataT::m31 * MatrixDataT::m42;
				tmp.m33 = MatrixDataT::m11 * MatrixDataT::m22 * MatrixDataT::m44 + MatrixDataT::m12 * MatrixDataT::m24 * MatrixDataT::m41 + MatrixDataT::m14 * MatrixDataT::m21 * MatrixDataT::m42 - MatrixDataT::m11 * MatrixDataT::m24 * MatrixDataT::m42 - MatrixDataT::m12 * MatrixDataT::m21 * MatrixDataT::m44 - MatrixDataT::m14 * MatrixDataT::m22 * MatrixDataT::m41;
				tmp.m34 = MatrixDataT::m11 * MatrixDataT::m24 * MatrixDataT::m32 + MatrixDataT::m12 * MatrixDataT::m21 * MatrixDataT::m34 + MatrixDataT::m14 * MatrixDataT::m22 * MatrixDataT::m31 - MatrixDataT::m11 * MatrixDataT::m22 * MatrixDataT::m34 - MatrixDataT::m12 * MatrixDataT::m24 * MatrixDataT::m31 - MatrixDataT::m14 * MatrixDataT::m21 * MatrixDataT::m32;

				tmp.m41 = MatrixDataT::m21 * MatrixDataT::m33 * MatrixDataT::m42 + MatrixDataT::m22 * MatrixDataT::m31 * MatrixDataT::m43 + MatrixDataT::m23 * MatrixDataT::m32 * MatrixDataT::m41 - MatrixDataT::m21 * MatrixDataT::m32 * MatrixDataT::m43 - MatrixDataT::m22 * MatrixDataT::m33 * MatrixDataT::m41 - MatrixDataT::m23 * MatrixDataT::m31 * MatrixDataT::m42;
				tmp.m42 = MatrixDataT::m11 * MatrixDataT::m32 * MatrixDataT::m43 + MatrixDataT::m12 * MatrixDataT::m33 * MatrixDataT::m41 + MatrixDataT::m13 * MatrixDataT::m31 * MatrixDataT::m42 - MatrixDataT::m11 * MatrixDataT::m33 * MatrixDataT::m42 - MatrixDataT::m12 * MatrixDataT::m31 * MatrixDataT::m43 - MatrixDataT::m13 * MatrixDataT::m32 * MatrixDataT::m41;
				tmp.m43 = MatrixDataT::m11 * MatrixDataT::m23 * MatrixDataT::m42 + MatrixDataT::m12 * MatrixDataT::m21 * MatrixDataT::m43 + MatrixDataT::m13 * MatrixDataT::m22 * MatrixDataT::m41 - MatrixDataT::m11 * MatrixDataT::m22 * MatrixDataT::m43 - MatrixDataT::m12 * MatrixDataT::m23 * MatrixDataT::m41 - MatrixDataT::m13 * MatrixDataT::m21 * MatrixDataT::m42;
				tmp.m44 = MatrixDataT::m11 * MatrixDataT::m22 * MatrixDataT::m33 + MatrixDataT::m12 * MatrixDataT::m23 * MatrixDataT::m31 + MatrixDataT::m13 * MatrixDataT::m21 * MatrixDataT::m32 - MatrixDataT::m11 * MatrixDataT::m23 * MatrixDataT::m32 - MatrixDataT::m12 * MatrixDataT::m21 * MatrixDataT::m33 - MatrixDataT::m13 * MatrixDataT::m22 * MatrixDataT::m31;

				return tmp / det;
			}

			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > IMatrix44< MatrixDataT >::changeHandside() const
			{
				return IMatrix44< MatrixDataT >(
					x, y, -z, w
					);
			}


			template< typename MatrixDataT >
			IMatrix44< MatrixDataT > operator * ( real_t lhs, const IMatrix44< MatrixDataT >& rhs )
			{
				return IMatrix44< MatrixDataT >( rhs * lhs );
			}

		}//namespace math

	}//namespace generic




}//namespace nc


#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MATRIX

#include "PLT_Matrix.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::Matrix33;
		using nc::generic::math::Matrix44;

	}//namespace math

}//namespace nc

#endif


namespace nc{

	namespace math{

		//å≈íËílçsóÒ

		static const Matrix33 Matrix33Zero( 0,0,0,
			0,0,0,
			0,0,0 ); 

		static const Matrix33 Matrix33Unit( 1,0,0,
			0,1,0,
			0,0,1	);

		static const Matrix44 Matrix44Zero( 0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0,0,0,0 ); 

		static const Matrix44 Matrix44Unit( 1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 ); 

	}//namespace math

}//namespace nc



#endif // __NC_MATRIX_h__