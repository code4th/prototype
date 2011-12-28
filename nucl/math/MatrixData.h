#ifndef __NC_MATRIXDATA_h__
#define __NC_MATRIXDATA_h__

#include "nucl/Types.h"
#include "Vector.h"
#include "nucl/Swizzle.h"

#include <memory>

using std::memcpy;

namespace nc{

	namespace math{

		/*
			[本来の行列の形]　m(行)(列) 
			| m11, m12, m13, m14 |
			| m21, m22, m23, m24 |
			| m31, m32, m33, m34 |
			| m41, m42, m43, m44 |
		*/

		/*
			[Row Major Matrix]の配列表現
			real_t [][] = { {00, 01, 02, 03}, {04, 05, 06, 07}, {08, 09, 10, 11}, {12, 13, 14, 15} };
		*/
		static const int ROW_MAJOR_TYPE_SIGNATURE = 0;

		/*
			[Col Major Matrix]の配列表現
			real_t [][] = { {00, 04, 08, 12}, {01, 05, 09, 13}, {02, 06, 10, 14}, {03, 07, 11, 15} };
		*/
		static const int COL_MAJOR_TYPE_SIGNATURE = 1;

		/*
			[TIPS]
			転置行列がColMajor/RowMajorの切り替えになる…ようですね。
		*/

	}//namespace math

	namespace generic {

		namespace math{



			struct Matrix33ColMajorData;
			struct Matrix33RowMajorData;

			struct Matrix44ColMajorData;
			struct Matrix44RowMajorData;

			struct Matrix33ColMajorData{

				enum{
					ROW = 3,
					COL = 3,
					VECTOR_SIZE = 3,
					NUM_OF_VECTOR = 3,
					MAJOR_TYPE_SIGNATURE = nc::math::COL_MAJOR_TYPE_SIGNATURE,
					OTHER_MAJOR_TYPE_SIGNATURE = nc::math::ROW_MAJOR_TYPE_SIGNATURE,
				};

				union{
					real_t	idx[ ROW * COL ];
					real_t	m[ COL ][ ROW ];

					//SoA
					//vec[0] == [ m11, m21, m31 ]
					//vec[1] == [ m12, m22, m32 ]
					//vec[2] == [ m13, m23, m33 ]
					struct{
						nc::math::Coordinate3Data vec[ NUM_OF_VECTOR ];
					};

					//SoA
					//x == [ m11, m21, m31 ]
					//y == [ m12, m22, m32 ]
					//z == [ m13, m23, m33 ]
					struct{
						nc::math::Coordinate3Data x, y, z;	
					};

					//Row Major
					struct{
						real_t m11, m21, m31;
						real_t m12, m22, m32;
						real_t m13, m23, m33;
					};
				};

				Matrix33ColMajorData();
				Matrix33ColMajorData( const Matrix33ColMajorData& rhs );
				Matrix33ColMajorData( const Matrix44ColMajorData& rhs );
				Matrix33ColMajorData( const Matrix33RowMajorData& rhs );
				Matrix33ColMajorData( const Matrix44RowMajorData& rhs );
				Matrix33ColMajorData( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33ColMajorData( const Coordinate3 arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33ColMajorData(	real_t m11, real_t m12, real_t m13,
										real_t m21, real_t m22, real_t m23,
										real_t m31, real_t m32, real_t m33 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix33ColMajorData( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );


				//代入
				Matrix33ColMajorData& assign( const Matrix33ColMajorData& rhs );
				Matrix33ColMajorData& assign( const Matrix44ColMajorData& rhs );

				Matrix33ColMajorData& assign( const Matrix33RowMajorData& rhs );
				Matrix33ColMajorData& assign( const Matrix44RowMajorData& rhs );

				Matrix33ColMajorData& assign( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33ColMajorData& assign( const Coordinate3 rhs[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33ColMajorData& assign(	real_t m11, real_t m12, real_t m13,
												real_t m21, real_t m22, real_t m23,
												real_t m31, real_t m32, real_t m33 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix33ColMajorData& assign( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );

				template< typename rhsT >
				const Matrix33ColMajorData& operator= ( const rhsT& rhs ){ return static_cast< Matrix33ColMajorData& >( assign(rhs) ); }

				//Matrix[行][列]　形式の値の取得をサポート
				Swizzle3<real_t> operator[]( int row );
				Swizzle3<const real_t> operator[]( int row ) const;

				real_t& operator()( uint32_t row, uint32_t col );
				const real_t& operator()( uint32_t row, uint32_t col ) const;

				//ColVector/RowVectorの取得
				const nc::math::Coordinate3Data rowVector( uint32_t row ) const;
				const nc::math::Coordinate3Data& colVector( uint32_t col ) const;

				Swizzle3< real_t > writableRowVector( uint32_t row );
				real_t * writableColVector( uint32_t col );

				//RowMajor/ColMajorの切り替え
				const Matrix33RowMajorData convToColMajorMatrix() const;
				const Matrix33ColMajorData& convToRowMajorMatrix() const;

				//転置行列を返します
				Matrix33ColMajorData transpose() const;
			};

			struct Matrix33RowMajorData{

				enum{
					ROW = 3,
					COL = 3,
					VECTOR_SIZE = 3,
					NUM_OF_VECTOR = 3,
					MAJOR_TYPE_SIGNATURE = nc::math::ROW_MAJOR_TYPE_SIGNATURE,
					OTHER_MAJOR_TYPE_SIGNATURE = nc::math::COL_MAJOR_TYPE_SIGNATURE,
				};

				union{
					real_t	idx[ COL * ROW ];
					real_t	m[ ROW ][ COL ];

					//AoS
					//vec[0] == [ m11, m12, m13 ]
					//vec[1] == [ m21, m22, m23 ]
					//vec[2] == [ m31, m32, m33 ]
					struct{
						nc::math::Coordinate3Data vec[ NUM_OF_VECTOR ];
					};

					//AoS
					//x == [ m11, m12, m31 ]
					//y == [ m21, m22, m23 ]
					//z == [ m31, m32, m33 ]
					struct{
						nc::math::Coordinate3Data x, y, z;	
					};

					//Col Major
					struct{
						real_t m11, m12, m13;
						real_t m21, m22, m23;
						real_t m31, m32, m33;
					};
				};

				Matrix33RowMajorData();
				Matrix33RowMajorData( const Matrix33RowMajorData& rhs );
				Matrix33RowMajorData( const Matrix44RowMajorData& rhs );
				Matrix33RowMajorData( const Matrix33ColMajorData& rhs );
				Matrix33RowMajorData( const Matrix44ColMajorData& rhs );
				Matrix33RowMajorData( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33RowMajorData( const Coordinate3 arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33RowMajorData(	real_t m11, real_t m12, real_t m13,
										real_t m21, real_t m22, real_t m23,
										real_t m31, real_t m32, real_t m33 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix33RowMajorData( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );


				//代入
				Matrix33RowMajorData& assign( const Matrix33RowMajorData& rhs );
				Matrix33RowMajorData& assign( const Matrix44RowMajorData& rhs );
				Matrix33RowMajorData& assign( const Matrix33ColMajorData& rhs );
				Matrix33RowMajorData& assign( const Matrix44ColMajorData& rhs );
				Matrix33RowMajorData& assign( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33RowMajorData& assign( const Coordinate3 rhs[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix33RowMajorData& assign(	real_t m11, real_t m12, real_t m13,
												real_t m21, real_t m22, real_t m23,
												real_t m31, real_t m32, real_t m33 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix33RowMajorData& assign( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );

				template< typename rhsT >
				const Matrix33RowMajorData& operator= ( const rhsT& rhs ){ return static_cast< Matrix33RowMajorData& >( assign(rhs) ); }

				//Matrix[行][列]　形式の値の取得をサポート
				real_t* operator[]( int row );
				const real_t* operator[]( int row ) const;

				real_t& operator()( uint32_t row, uint32_t col );
				const real_t& operator()( uint32_t row, uint32_t col ) const;

				//ColVector/RowVectorの取得
				const nc::math::Coordinate3Data& rowVector( uint32_t row ) const;
				const nc::math::Coordinate3Data colVector( uint32_t col ) const;

				real_t* writableRowVector( uint32_t row );
				Swizzle3< real_t > writableColVector( uint32_t col );

				//RowMajor/ColMajorの切り替え
				const Matrix33RowMajorData& convToColMajorMatrix() const;
				const Matrix33ColMajorData convToRowMajorMatrix() const;

				//転置行列を返します
				Matrix33RowMajorData transpose() const;
			};


			struct Matrix44ColMajorData{
				enum{
					ROW = 4,
					COL = 4,
					VECTOR_SIZE = 4,
					NUM_OF_VECTOR = 4,
					MAJOR_TYPE_SIGNATURE = nc::math::COL_MAJOR_TYPE_SIGNATURE,
					OTHER_MAJOR_TYPE_SIGNATURE = nc::math::ROW_MAJOR_TYPE_SIGNATURE,
				};

				union{
					real_t	idx[ ROW * COL ];
					real_t	m[ COL ][ ROW ];
					
					//SoA
					//vec[0] == [ m11, m21, m31, m41 ]
					//vec[1] == [ m12, m22, m32, m42 ]
					//vec[2] == [ m13, m23, m33, m43 ]
					//vec[3] == [ m14, m24, m34, m44 ]
					struct{
						nc::math::Coordinate4Data vec[ NUM_OF_VECTOR ];
					};

					//SoA
					//x == [ m11, m21, m31, m41 ]
					//y == [ m12, m22, m32, m42 ]
					//z == [ m13, m23, m33, m43 ]
					//w == [ m14, m24, m34, m44 ]
					struct{
						nc::math::Coordinate4Data x, y, z, w;	//the vector is col vector if the matrix is col major.
														//and the vector is row vector if the matrix is row major.
					};
					
					//Row Major
					struct{
						real_t m11, m21, m31, m41;
						real_t m12, m22, m32, m42;
						real_t m13, m23, m33, m43;
						real_t m14, m24, m34, m44;
					};
				};


				Matrix44ColMajorData();
				Matrix44ColMajorData( const Matrix44ColMajorData& rhs );
				Matrix44ColMajorData( const Matrix33ColMajorData& rhs );
				Matrix44ColMajorData( const Matrix44RowMajorData& rhs );
				Matrix44ColMajorData( const Matrix33RowMajorData& rhs );
				Matrix44ColMajorData( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44ColMajorData( const Coordinate4 arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44ColMajorData(	real_t m11, real_t m12, real_t m13, real_t m14,
										real_t m21, real_t m22, real_t m23, real_t m24,
										real_t m31, real_t m32, real_t m33, real_t m34,
										real_t m41, real_t m42, real_t m43, real_t m44 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix44ColMajorData( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );


				//代入
				Matrix44ColMajorData& assign( const Matrix44ColMajorData& rhs );
				Matrix44ColMajorData& assign( const Matrix33ColMajorData& rhs );
				Matrix44ColMajorData& assign( const Matrix44RowMajorData& rhs );
				Matrix44ColMajorData& assign( const Matrix33RowMajorData& rhs );
				Matrix44ColMajorData& assign( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44ColMajorData& assign( const Coordinate4 rhs[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44ColMajorData& assign(	real_t m11, real_t m12, real_t m13, real_t m14,
												real_t m21, real_t m22, real_t m23, real_t m24,
												real_t m31, real_t m32, real_t m33, real_t m34,
												real_t m41, real_t m42, real_t m43, real_t m44 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix44ColMajorData& assign( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );

				template< typename rhsT >
				const Matrix44ColMajorData& operator= ( const rhsT& rhs ){ return static_cast< Matrix44ColMajorData& >( assign(rhs) ); }

				//Matrix[行][列]　形式の値の取得をサポート
				Swizzle4<real_t> operator[]( int row );
				Swizzle4<const real_t> operator[]( int row ) const;
				
				real_t& operator()( uint32_t row, uint32_t col );
				const real_t& operator()( uint32_t row, uint32_t col ) const;

				//ColVector/RowVectorの取得
				const nc::math::Coordinate4Data rowVector( uint32_t row ) const;
				const nc::math::Coordinate4Data& colVector( uint32_t col ) const;

				Swizzle4< real_t > writableRowVector( uint32_t row );
				real_t * writableColVector( uint32_t col );

				//RowMajor/ColMajorの切り替え
				const Matrix44RowMajorData convToColMajorMatrix() const;
				const Matrix44ColMajorData& convToRowMajorMatrix() const;

				//転置行列を返します
				Matrix44ColMajorData transpose() const;

			};

			struct Matrix44RowMajorData{

				enum{
					ROW = 4,
					COL = 4,
					VECTOR_SIZE = 4,
					NUM_OF_VECTOR = 4,
					MAJOR_TYPE_SIGNATURE = nc::math::ROW_MAJOR_TYPE_SIGNATURE,
					OTHER_MAJOR_TYPE_SIGNATURE = nc::math::COL_MAJOR_TYPE_SIGNATURE,
				};

				union{
					real_t	idx[ COL * ROW ];
					real_t	m[ ROW ][ COL ];

					//AoS
					//vec[0] == [ m11, m12, m13, m14 ]
					//vec[1] == [ m21, m22, m23, m24 ]
					//vec[2] == [ m31, m32, m33, m34 ]
					//vec[3] == [ m31, m32, m33, m44 ]
					struct{
						nc::math::Coordinate4Data vec[ NUM_OF_VECTOR ];
					};

					//AoS
					//x == [ m11, m12, m13, m14 ]
					//y == [ m21, m22, m23, m24 ]
					//z == [ m31, m32, m33, m34 ]
					//z == [ m41, m42, m43, m44 ]
					struct{
						nc::math::Coordinate4Data x, y, z, w;	//the vector is col vector if the matrix is col major.
														//and the vector is row vector if the matrix is row major.
					};

					//Col Major
					struct{
						real_t m11, m12, m13, m14;
						real_t m21, m22, m23, m24;
						real_t m31, m32, m33, m34;
						real_t m41, m42, m43, m44;
					};
				};

				Matrix44RowMajorData();
				Matrix44RowMajorData( const Matrix44RowMajorData& rhs );
				Matrix44RowMajorData( const Matrix33RowMajorData& rhs );
				Matrix44RowMajorData( const Matrix44ColMajorData& rhs );
				Matrix44RowMajorData( const Matrix33ColMajorData& rhs );
				Matrix44RowMajorData( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44RowMajorData( const Coordinate4 arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44RowMajorData(	real_t m11, real_t m12, real_t m13, real_t m14,
					real_t m21, real_t m22, real_t m23, real_t m24,
					real_t m31, real_t m32, real_t m33, real_t m34,
					real_t m41, real_t m42, real_t m43, real_t m44 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix44RowMajorData( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );


				//代入
				Matrix44RowMajorData& assign( const Matrix44RowMajorData& rhs );
				Matrix44RowMajorData& assign( const Matrix33RowMajorData& rhs );
				Matrix44RowMajorData& assign( const Matrix44ColMajorData& rhs );
				Matrix44RowMajorData& assign( const Matrix33ColMajorData& rhs );
				Matrix44RowMajorData& assign( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44RowMajorData& assign( const Coordinate4 rhs[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE );
				Matrix44RowMajorData& assign(	real_t m11, real_t m12, real_t m13, real_t m14,
					real_t m21, real_t m22, real_t m23, real_t m24,
					real_t m31, real_t m32, real_t m33, real_t m34,
					real_t m41, real_t m42, real_t m43, real_t m44 );

				//配列arrをimportMajorTypeSignatureで指定した行列の形としてインポートします。
				Matrix44RowMajorData& assign( const real_t arr[], int importMajorTypeSignature = MAJOR_TYPE_SIGNATURE  );

				template< typename rhsT >
				const Matrix44RowMajorData& operator= ( const rhsT& rhs ){ return static_cast< Matrix44RowMajorData& >( assign(rhs) ); }
				
				//operator real_t*();
				//operator const real_t*() const;

				//Matrix[行][列]　形式の値の取得をサポート
				real_t* operator[]( int row );
				const real_t* operator[]( int row ) const;

				real_t& operator()( uint32_t row, uint32_t col );
				const real_t& operator()( uint32_t row, uint32_t col ) const;

				//ColVector/RowVectorの取得
				const nc::math::Coordinate4Data& rowVector( uint32_t row ) const;
				const nc::math::Coordinate4Data colVector( uint32_t col ) const;

				real_t* writableRowVector( uint32_t row );
				Swizzle4< real_t > writableColVector( uint32_t col );

				//RowMajor/ColMajorの切り替え
				const Matrix44RowMajorData& convToColMajorMatrix() const;
				const Matrix44ColMajorData convToRowMajorMatrix() const;

				//転置行列を返します
				Matrix44RowMajorData transpose() const;

			};




			struct Matrix33Data : public Matrix33ColMajorData{

			};

			struct Matrix44Data : public Matrix44ColMajorData{

			};



			//--------------------inline impliment--------------------------

			


			inline Matrix33ColMajorData::Matrix33ColMajorData()
			{

			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const Matrix33ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const Matrix44ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const Matrix33RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const Matrix44RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature )
			{
				assign( x, y, z, importMajorTypeSignature );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const Coordinate3 arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( real_t m11, real_t m12, real_t m13, real_t m21, real_t m22, real_t m23, real_t m31, real_t m32, real_t m33 )
			{
				assign( m11, m12, m13, 
					m21, m22, m23,
					m31, m32, m33 );
			}

			inline Matrix33ColMajorData::Matrix33ColMajorData( const real_t arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}


			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const Matrix33ColMajorData& rhs )
			{
				memcpy( idx, rhs.idx, sizeof(idx) );
				return *this;
			}

			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const Matrix44ColMajorData& rhs )
			{
				x = rhs.x.orthoProjection();
				y = rhs.y.orthoProjection();
				z = rhs.z.orthoProjection();
				return *this;
			}


			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const Matrix33RowMajorData& rhs )
			{
				assign( rhs.convToRowMajorMatrix() );
				return *this;
			}

			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const Matrix44RowMajorData& rhs )
			{
				assign( rhs.convToRowMajorMatrix() );
				return *this;
			}

			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature)
			{
				this->x = x;
				this->y = y;
				this->z = z;

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const Coordinate3 rhs[], int importMajorTypeSignature )
			{
				this->x = rhs[0];
				this->y = rhs[1];
				this->z = rhs[2];

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( real_t m11, real_t m12, real_t m13, real_t m21, real_t m22, real_t m23, real_t m31, real_t m32, real_t m33 )
			{
				this->m11 = m11;
				this->m12 = m12;
				this->m13 = m13;

				this->m21 = m21;
				this->m22 = m22;
				this->m23 = m23;

				this->m31 = m31;
				this->m32 = m32;
				this->m33 = m33;
				return *this;
			}

			inline Matrix33ColMajorData& Matrix33ColMajorData::assign( const real_t arr[], int importMajorTypeSignature )
			{
				memcpy( idx, arr, sizeof(idx) );

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}

				return *this;
			}

			inline Swizzle3<real_t> Matrix33ColMajorData::operator[]( int row )
			{
				real_t* startAddr = &m11 + row;
				return Swizzle3<real_t>( *(startAddr), *(startAddr+COL), *(startAddr+(COL*2)) );
			}

			inline Swizzle3<const real_t> Matrix33ColMajorData::operator[]( int row ) const
			{
				const real_t* startAddr = &m11 + row;
				return Swizzle3<const real_t>( *(startAddr), *(startAddr+COL), *(startAddr+(COL*2)) );
			}


			inline real_t& Matrix33ColMajorData::operator()( uint32_t row, uint32_t col )
			{
				return m[ col ][ row ];
			}

			inline const real_t& Matrix33ColMajorData::operator()( uint32_t row, uint32_t col ) const
			{
				return m[ col ][ row ];
			}


			inline const nc::math::Coordinate3Data Matrix33ColMajorData::rowVector( uint32_t row ) const
			{
				return nc::math::Coordinate3Data().assign( *(&m11 + row), *(&m12 + row), *(&m13 + row) );
			}

			inline const nc::math::Coordinate3Data& Matrix33ColMajorData::colVector( uint32_t col ) const
			{
				return vec[ col ];
			}

			inline Swizzle3< real_t > Matrix33ColMajorData::writableRowVector( uint32_t row )
			{
				return Swizzle3< real_t >( m[ 0 ][ row ], m[ 1 ][ row ], m[ 2 ][ row ] );
			}

			inline real_t* Matrix33ColMajorData::writableColVector( uint32_t col )
			{
				return m[ col ];
			}

			inline const Matrix33RowMajorData Matrix33ColMajorData::convToColMajorMatrix() const
			{
				return Matrix33RowMajorData( 
					m11, m12, m13,
					m21, m22, m23,
					m31, m32, m33
					);
			}

			inline const Matrix33ColMajorData& Matrix33ColMajorData::convToRowMajorMatrix() const
			{
				return *this;
			}

			inline Matrix33ColMajorData Matrix33ColMajorData::transpose() const
			{
				return Matrix33ColMajorData(
					m11, m21, m31,
					m12, m22, m32,
					m13, m23, m33
					);
			}

			//------------------------------------------------

			inline Matrix33RowMajorData::Matrix33RowMajorData()
			{

			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const Matrix33RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const Matrix44RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const Matrix33ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const Matrix44ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature )
			{
				assign( x, y, z, importMajorTypeSignature );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const Coordinate3 arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( real_t m11, real_t m12, real_t m13, real_t m21, real_t m22, real_t m23, real_t m31, real_t m32, real_t m33 )
			{
				assign( m11, m12, m13, 
					m21, m22, m23,
					m31, m32, m33 );
			}

			inline Matrix33RowMajorData::Matrix33RowMajorData( const real_t arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}


			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const Matrix33RowMajorData& rhs )
			{
				memcpy( idx, rhs.idx, sizeof(idx) );
				return *this;
			}

			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const Matrix44RowMajorData& rhs )
			{
				x = rhs.x.orthoProjection();
				y = rhs.y.orthoProjection();
				z = rhs.z.orthoProjection();
				return *this;
			}


			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const Matrix33ColMajorData& rhs )
			{
				assign( rhs.convToColMajorMatrix() );
				return *this;
			}

			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const Matrix44ColMajorData& rhs )
			{
				assign( rhs.convToColMajorMatrix() );
				return *this;
			}

			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const Coordinate3& x, const Coordinate3& y, const Coordinate3& z, int importMajorTypeSignature )
			{
				this->x = x;
				this->y = y;
				this->z = z;

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const Coordinate3 rhs[], int importMajorTypeSignature )
			{
				this->x = rhs[0];
				this->y = rhs[1];
				this->z = rhs[2];

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( real_t m11, real_t m12, real_t m13, real_t m21, real_t m22, real_t m23, real_t m31, real_t m32, real_t m33 )
			{
				this->m11 = m11;
				this->m12 = m12;
				this->m13 = m13;

				this->m21 = m21;
				this->m22 = m22;
				this->m23 = m23;

				this->m31 = m31;
				this->m32 = m32;
				this->m33 = m33;
				return *this;
			}

			inline Matrix33RowMajorData& Matrix33RowMajorData::assign( const real_t arr[], int importMajorTypeSignature )
			{
				memcpy( idx, arr, sizeof(idx) );

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}

				return *this;
			}

			inline real_t* Matrix33RowMajorData::operator[]( int row )
			{
				return &m11 + (row * COL);
			}

			inline const real_t* Matrix33RowMajorData::operator[]( int row ) const
			{
				return &m11 + (row * COL);

			}
			inline real_t& Matrix33RowMajorData::operator()( uint32_t row, uint32_t col )
			{
				return m[ row ][ col ];
			}

			inline const real_t& Matrix33RowMajorData::operator()( uint32_t row, uint32_t col ) const
			{
				return m[ row ][ col ];
			}

			inline real_t* Matrix33RowMajorData::writableRowVector( uint32_t row )
			{
				return m[ row ];
			}

			inline Swizzle3< real_t > Matrix33RowMajorData::writableColVector( uint32_t col )
			{
				return Swizzle3< real_t >( m[ 0 ][ col ], m[ 1 ][ col ], m[ 2 ][ col ] );
			}


			inline const nc::math::Coordinate3Data& Matrix33RowMajorData::rowVector( uint32_t row ) const
			{
				return vec[ row ];
			}

			inline const nc::math::Coordinate3Data Matrix33RowMajorData::colVector( uint32_t col ) const
			{
				return nc::math::Vector3( *(&m11 + col), *(&m21 + col), *(&m31 + col) );
			}

			inline const Matrix33RowMajorData& Matrix33RowMajorData::convToColMajorMatrix() const
			{
				return *this;
			}

			inline const Matrix33ColMajorData Matrix33RowMajorData::convToRowMajorMatrix() const
			{
				return Matrix33ColMajorData( 
					m11, m12, m13,
					m21, m22, m23,
					m31, m32, m33
					);
			}

			inline Matrix33RowMajorData Matrix33RowMajorData::transpose() const
			{
				return Matrix33RowMajorData(
					m11, m21, m31,
					m12, m22, m32,
					m13, m23, m33
					);
			}


			//------------------------------------------------


			inline Matrix44ColMajorData::Matrix44ColMajorData()
			{

			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const Matrix44ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const Matrix33ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const Matrix44RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const Matrix33RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature )
			{
				assign( x, y, z, w, importMajorTypeSignature );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const Coordinate4 arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData(	real_t m11, real_t m12, real_t m13, real_t m14,
				real_t m21, real_t m22, real_t m23, real_t m24,
				real_t m31, real_t m32, real_t m33, real_t m34,
				real_t m41, real_t m42, real_t m43, real_t m44 )
			{
				assign( m11, m12, m13, m14, 
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44 );
			}

			inline Matrix44ColMajorData::Matrix44ColMajorData( const real_t arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const Matrix44ColMajorData& rhs )
			{
				memcpy( idx, rhs.idx, sizeof(idx) );
				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const Matrix33ColMajorData& rhs )
			{
				x.assign( rhs.x, 0 );
				y.assign( rhs.y, 0 );
				z.assign( rhs.z, 0 );
				w.assign( 0, 0, 0, 1 );
				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const Matrix33RowMajorData& rhs )
			{
				assign( rhs.convToRowMajorMatrix() );
				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const Matrix44RowMajorData& rhs )
			{
				assign( rhs.convToRowMajorMatrix() );
				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature )
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}

				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const Coordinate4 rhs[], int importMajorTypeSignature )
			{
				this->x = rhs[0];
				this->y = rhs[1];
				this->z = rhs[2];
				this->w = rhs[3];

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign(	real_t m11, real_t m12, real_t m13, real_t m14,
				real_t m21, real_t m22, real_t m23, real_t m24,
				real_t m31, real_t m32, real_t m33, real_t m34,
				real_t m41, real_t m42, real_t m43, real_t m44 )
			{
				this->m11 = m11;
				this->m12 = m12;
				this->m13 = m13;
				this->m14 = m14;

				this->m21 = m21;
				this->m22 = m22;
				this->m23 = m23;
				this->m24 = m24;

				this->m31 = m31;
				this->m32 = m32;
				this->m33 = m33;
				this->m34 = m34;

				this->m41 = m41;
				this->m42 = m42;
				this->m43 = m43;
				this->m44 = m44;

				return *this;
			}

			inline Matrix44ColMajorData& Matrix44ColMajorData::assign( const real_t arr[], int importMajorTypeSignature )
			{
				memcpy( idx, arr, sizeof(idx) );

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Swizzle4<real_t> Matrix44ColMajorData::operator[]( int row )
			{
				real_t* startAddr = &m11 + row;
				return Swizzle4<real_t>( *(startAddr), *(startAddr+COL), *(startAddr+(COL*2)), *(startAddr+(COL*3)) );
			}

			inline Swizzle4<const real_t> Matrix44ColMajorData::operator[]( int row ) const
			{
				const real_t* startAddr = &m11 + row;
				return Swizzle4<const real_t>( *(startAddr), *(startAddr+COL), *(startAddr+(COL*2)), *(startAddr+(COL*3)) );
			}

			inline real_t& Matrix44ColMajorData::operator()( uint32_t row, uint32_t col )
			{
				return m[ col ][ row ];
			}

			inline const real_t& Matrix44ColMajorData::operator()( uint32_t row, uint32_t col ) const
			{
				return m[ col ][ row ];
			}


			inline const nc::math::Coordinate4Data Matrix44ColMajorData::rowVector( uint32_t row ) const
			{
				return nc::math::Coordinate4Data().assign( *(&m11 + row), *(&m12 + row), *(&m13 + row), *(&m14 + row) );
			}

			inline const nc::math::Coordinate4Data& Matrix44ColMajorData::colVector( uint32_t col ) const
			{
				return vec[ col ];
			}

			inline Swizzle4< real_t > Matrix44ColMajorData::writableRowVector( uint32_t row )
			{
				return Swizzle4< real_t >( m[ 0 ][ row ], m[ 1 ][ row ], m[ 2 ][ row ], m[ 3 ][ row ] );
			}

			inline real_t* Matrix44ColMajorData::writableColVector( uint32_t col )
			{
				return m[ col ];
			}

			inline const Matrix44RowMajorData Matrix44ColMajorData::convToColMajorMatrix() const
			{
				return Matrix44RowMajorData( 
					m11, m12, m13, m14,
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44
					);
			}

			inline const Matrix44ColMajorData& Matrix44ColMajorData::convToRowMajorMatrix() const
			{
				return *this;
			}

			inline Matrix44ColMajorData Matrix44ColMajorData::transpose() const
			{
				return Matrix44ColMajorData(
					m11, m21, m31, m41,
					m12, m22, m32, m42,
					m13, m23, m33, m43,
					m14, m24, m34, m44
					);
			}

			//------------------------------------------------


			inline Matrix44RowMajorData::Matrix44RowMajorData()
			{

			}

			inline Matrix44RowMajorData::Matrix44RowMajorData( const Matrix44RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44RowMajorData::Matrix44RowMajorData( const Matrix33RowMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44RowMajorData::Matrix44RowMajorData( const Matrix44ColMajorData& rhs )
			{
				assign( rhs );
			}

			inline Matrix44RowMajorData::Matrix44RowMajorData( const Matrix33ColMajorData& rhs )
			{
				assign( rhs );
			}


			inline Matrix44RowMajorData::Matrix44RowMajorData( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature )
			{
				assign( x, y, z, w, importMajorTypeSignature );
			}

			inline Matrix44RowMajorData::Matrix44RowMajorData( const Coordinate4 arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}

			inline Matrix44RowMajorData::Matrix44RowMajorData(	real_t m11, real_t m12, real_t m13, real_t m14,
				real_t m21, real_t m22, real_t m23, real_t m24,
				real_t m31, real_t m32, real_t m33, real_t m34,
				real_t m41, real_t m42, real_t m43, real_t m44 )
			{
				assign( m11, m12, m13, m14, 
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44 );
			}

			inline Matrix44RowMajorData::Matrix44RowMajorData( const real_t arr[], int importMajorTypeSignature )
			{
				assign( arr, importMajorTypeSignature );
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const Matrix44RowMajorData& rhs )
			{
				memcpy( idx, rhs.idx, sizeof(idx) );
				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const Matrix33RowMajorData& rhs )
			{
				x.assign( rhs.x, 0 );
				y.assign( rhs.y, 0 );
				z.assign( rhs.z, 0 );
				w.assign( 0, 0, 0, 1 );
				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const Matrix33ColMajorData& rhs )
			{
				assign( rhs.convToColMajorMatrix() );
				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const Matrix44ColMajorData& rhs )
			{
				assign( rhs.convToColMajorMatrix() );
				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const Coordinate4& x, const Coordinate4& y, const Coordinate4& z, const Coordinate4& w, int importMajorTypeSignature )
			{
				this->x = x;
				this->y = y;
				this->z = z;
				this->w = w;

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const Coordinate4 rhs[], int importMajorTypeSignature )
			{
				this->x = rhs[0];
				this->y = rhs[1];
				this->z = rhs[2];
				this->w = rhs[3];

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign(	real_t m11, real_t m12, real_t m13, real_t m14,
				real_t m21, real_t m22, real_t m23, real_t m24,
				real_t m31, real_t m32, real_t m33, real_t m34,
				real_t m41, real_t m42, real_t m43, real_t m44 )
			{
				this->m11 = m11;
				this->m12 = m12;
				this->m13 = m13;
				this->m14 = m14;

				this->m21 = m21;
				this->m22 = m22;
				this->m23 = m23;
				this->m24 = m24;

				this->m31 = m31;
				this->m32 = m32;
				this->m33 = m33;
				this->m34 = m34;

				this->m41 = m41;
				this->m42 = m42;
				this->m43 = m43;
				this->m44 = m44;

				return *this;
			}

			inline Matrix44RowMajorData& Matrix44RowMajorData::assign( const real_t arr[], int importMajorTypeSignature )
			{
				memcpy( idx, arr, sizeof(idx) );

				//違うタイプなら変換
				if( importMajorTypeSignature != MAJOR_TYPE_SIGNATURE )
				{
					assign( transpose() );
				}
				return *this;
			}

			inline real_t* Matrix44RowMajorData::operator[]( int row )
			{
				return m[ row ];
			}

			inline const real_t* Matrix44RowMajorData::operator[]( int row ) const
			{
				return m[ row ];
			}

			inline real_t& Matrix44RowMajorData::operator()( uint32_t row, uint32_t col )
			{
				return m[ row ][ col ];
			}

			inline const real_t& Matrix44RowMajorData::operator()( uint32_t row, uint32_t col ) const
			{
				return m[ row ][ col ];
			}

			inline real_t* Matrix44RowMajorData::writableRowVector( uint32_t row )
			{
				return m[ row ];
			}

			inline Swizzle4< real_t > Matrix44RowMajorData::writableColVector( uint32_t col )
			{
				return Swizzle4< real_t >( m[ 0 ][ col ], m[ 1 ][ col ], m[ 2 ][ col ], m[ 3 ][ col ] );
			}


			inline const nc::math::Coordinate4Data& Matrix44RowMajorData::rowVector( uint32_t row ) const
			{
				return vec[ row ];
			}

			inline const nc::math::Coordinate4Data Matrix44RowMajorData::colVector( uint32_t col ) const
			{
				return nc::math::Coordinate4Data().assign( *(&m11 + col), *(&m21 + col), *(&m31 + col), *(&m41 + col) );
			}


			inline const Matrix44RowMajorData& Matrix44RowMajorData::convToColMajorMatrix() const
			{
				return *this;
			}

			inline const Matrix44ColMajorData Matrix44RowMajorData::convToRowMajorMatrix() const
			{
				return Matrix44ColMajorData( 
					m11, m12, m13, m14,
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44
					);
			}

			inline Matrix44RowMajorData Matrix44RowMajorData::transpose() const
			{
				return Matrix44RowMajorData(
					m11, m21, m31, m41,
					m12, m22, m32, m42,
					m13, m23, m33, m43,
					m14, m24, m34, m44
					);
			}



		}//namespace math
	}//namespace generic
}//namespace nc

#include "PlatformInclude.h"
#ifdef NC_PLATFORM_USE_INTRINSIC_MATRIX_DATA

#include "PLT_MatrixData.h"

#else

namespace nc{

	namespace math{

		using nc::generic::math::Matrix33ColMajorData;
		using nc::generic::math::Matrix33RowMajorData;

		using nc::generic::math::Matrix44ColMajorData;
		using nc::generic::math::Matrix44RowMajorData;

		using nc::generic::math::Matrix33Data;
		using nc::generic::math::Matrix44Data;

	}//namespace math

}//namespace nc

#endif



#endif // __NC_MATRIXDATA_h__