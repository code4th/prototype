#ifndef PlatformInclude_h__
#define PlatformInclude_h__

#define WIN32_LEAN_AND_MEAN

//sa)  http://msdn.microsoft.com/ja-jp/library/aa383745%28en-us,VS.85%29.aspx

#ifndef _WIN32_WINNT
#define _WIN32_WINNT			0x0501 //_WIN32_WINNT_WINXP
#endif

#include <Windows.h>

//-----------------------------------
// Nucl Configs
//-----------------------------------

//---------------[ Core Section ]----------------

//組み込み基本型を使う
//#define NC_PLATFORM_USE_INTRINSIC_NATIVE_TYPES


//---------------[ Thread Section ]----------------

//組み込みのスレッドを使用する
//generic仕様としてpthreadを用いた物があります。
//pthreadがサポートされている環境の場合は、genericな実装が利用できます。
#define NC_PLATFORM_USE_INTRINSIC_THREAD

//組み込みのMUTEXを使用する
//generic仕様としてpthreadを用いた物があります。
//pthreadがサポートされている環境の場合は、genericな実装が利用できます。
#define NC_PLATFORM_USE_INTRINSIC_MUTEX

//組み込みのスレッドを使用する（generic仕様が無い為、必須です。）
//#define NC_PLATFORM_USE_INTRINSIC_THREAD

//組み込みアトミック関数
//#define NC_PLATFORM_USE_INTRINSIC_ATOMIC

//組み込みユニバーサル関数
#define NC_PLATFORM_USE_INTRINSIC_INTERLOCKED


//---------------[ Thread Section ]----------------

//newのオーバーロード
#define NC_PLATFORM_USE_INTRINSIC_NEW

//---------------[ Math Section ]----------------

//組み込みCOORDINATE DATA
//#define NC_PLATFORM_USE_INTRINSIC_COORDINATE_DATA

//#define NC_PLATFORM_USE_INTRINSIC_COORDINATE

#define NC_PLATFORM_USE_INTRINSIC_VECTOR

//#define NC_PLATFORM_USE_INTRINSIC_VECTOR_UTIL

//#define NC_PLATFORM_USE_INTRINSIC_QUATERNION

//#define NC_PLATFORM_USE_INTRINSIC_MATRIX_DATA

#define NC_PLATFORM_USE_INTRINSIC_MATRIX

#define NC_PLATFORM_USE_INTRINSIC_MATRIX_UTIL

//#define NC_PLATFORM_USE_INTRINSIC_POINT

#endif // PlatformInclude_h__
