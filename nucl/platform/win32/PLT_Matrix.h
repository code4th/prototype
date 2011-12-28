#ifndef __NC_PLT_MATRIX_h__
#define __NC_PLT_MATRIX_h__

namespace nc{

	namespace platform{

		namespace math{

			class Matrix33 : public nc::generic::math::Matrix33{
			public:

				Matrix33() {};
				Matrix33( const Matrix33& rhs )	: nc::generic::math::Matrix33( rhs ){}
				//Matrix33( const Matrix44& rhs )	: nc::generic::math::Matrix33(( rhs ) ){}
				//Matrix33( const Matrix44& rhs )	{ assign( rhs ); }
				Matrix33( const nc::math::Matrix33ColMajorData& rhs ) : nc::generic::math::Matrix33( rhs ) {}
				Matrix33( const nc::math::Matrix33RowMajorData& rhs ) : nc::generic::math::Matrix33( rhs ) {}
				Matrix33( const nc::math::Matrix44ColMajorData& rhs ) : nc::generic::math::Matrix33( rhs ) {}
				Matrix33( const nc::math::Matrix44RowMajorData& rhs ) : nc::generic::math::Matrix33( rhs ) {}

				Matrix33( const nc::math::Coordinate3& x, const nc::math::Coordinate3& y, const nc::math::Coordinate3& z, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE ) : nc::generic::math::Matrix33( x,y,z,importMajorTypeSignature ) {}
				Matrix33( const nc::math::Coordinate3 arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  ) : nc::generic::math::Matrix33( arr, importMajorTypeSignature ){}
				Matrix33(	real_t m11, real_t m12, real_t m13, 
							real_t m21, real_t m22, real_t m23,
							real_t m31, real_t m32, real_t m33 ) : nc::generic::math::Matrix33( m11,m12,m13, m21,m22,m23, m31,m32,m33 ){} 
				Matrix33( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE ) : nc::generic::math::Matrix33( arr, importMajorTypeSignature ){}

				template< typename rhsT >
				const Matrix33& operator= ( const rhsT& rhs ){ return static_cast< Matrix33& >( assign(rhs) ); }

				//íPçÄ +/-ââéZéq
				Matrix33 operator+ () const { return nc::generic::math::Matrix33::operator+(); }
				Matrix33 operator- () const { return nc::generic::math::Matrix33::operator-(); }

				//â¡å∏éZÇ∆ä|ÇØéZ
				Matrix33 operator* ( const Matrix33& rhs ) const;
				Matrix33 operator+ ( const Matrix33& rhs ) const;
				Matrix33 operator- ( const Matrix33& rhs ) const;
				const Matrix33& operator*=( const Matrix33& rhs );
				const Matrix33& operator+=( const Matrix33& rhs );
				const Matrix33& operator-=( const Matrix33& rhs );

				//ÉXÉJÉâÅ[î{
				Matrix33 operator* ( real_t bias ) const;
				Matrix33 operator/ ( real_t bias ) const;
				const Matrix33& operator*=( real_t bias );
				const Matrix33& operator/=( real_t bias );
			};

			class Matrix44 : public nc::generic::math::Matrix44{
			public:

				Matrix44() {};
				Matrix44( const Matrix44& rhs )	: nc::generic::math::Matrix44( rhs ){}
				//Matrix44( const Matrix33& rhs )	: nc::generic::math::Matrix44( rhs ){}
				Matrix44( const nc::math::Matrix33ColMajorData& rhs ) : nc::generic::math::Matrix44( rhs ) {}
				Matrix44( const nc::math::Matrix33RowMajorData& rhs ) : nc::generic::math::Matrix44( rhs ) {}
				Matrix44( const nc::math::Matrix44ColMajorData& rhs ) : nc::generic::math::Matrix44( rhs ) {}
				Matrix44( const nc::math::Matrix44RowMajorData& rhs ) : nc::generic::math::Matrix44( rhs ) {}

				Matrix44( const nc::math::Coordinate4& x, const nc::math::Coordinate4& y, const nc::math::Coordinate4& z, const nc::math::Coordinate4& w, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE ) : nc::generic::math::Matrix44( x,y,z,w,importMajorTypeSignature ) {}
				Matrix44( const nc::math::Coordinate4 arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  ) : nc::generic::math::Matrix44( arr, importMajorTypeSignature ){}
				Matrix44(	real_t m11, real_t m12, real_t m13, real_t m14,
				real_t m21, real_t m22, real_t m23, real_t m24,
				real_t m31, real_t m32, real_t m33, real_t m34,
				real_t m41, real_t m42, real_t m43, real_t m44 ) : nc::generic::math::Matrix44( m11,m12,m13,m14, m21,m22,m23,m24, m31,m32,m33,m34, m41,m42,m43,m44 ){} 
				Matrix44( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE ) : nc::generic::math::Matrix44( arr, importMajorTypeSignature ){}

				template< typename rhsT >
				const Matrix44& operator= ( const rhsT& rhs ){ return static_cast< Matrix44& >( assign(rhs) ); }

				//íPçÄ +/-ââéZéq
				Matrix44 operator+ () const { return nc::generic::math::Matrix44::operator+(); }
				Matrix44 operator- () const { return nc::generic::math::Matrix44::operator-(); }

				//â¡å∏éZÇ∆ä|ÇØéZ
				Matrix44 operator* ( const Matrix44& rhs ) const;
				Matrix44 operator+ ( const Matrix44& rhs ) const;
				Matrix44 operator- ( const Matrix44& rhs ) const;
				const Matrix44& operator*=( const Matrix44& rhs );
				const Matrix44& operator+=( const Matrix44& rhs );
				const Matrix44& operator-=( const Matrix44& rhs );


				//ÉXÉJÉâÅ[î{
				Matrix44 operator* ( real_t bias ) const;
				Matrix44 operator/ ( real_t bias ) const;
				const Matrix44& operator*=( real_t bias );
				const Matrix44& operator/=( real_t bias );

			};

		}//namespace math

	}//namespace platform

	namespace math{

		using nc::platform::math::Matrix33;
		using nc::platform::math::Matrix44;

	}//namespace math

}//namespace nc


#endif // __NC_PLT_MATRIX_h__