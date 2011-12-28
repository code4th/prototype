
#if !defined( NC_NC_CONFIG_H )
#define NC_NC_CONFIG_H 

#define NC_DEBUG

/*
スマートポインタのカウンターメモリの確保にstd::allocatorを使用します。
*/
//#define NC_SP_USE_STD_ALLOCATOR

/*
C++0xの機能です。しばらく使えません。
[参考]
http://d.hatena.ne.jp/faith_and_brave/20071004/1191497817
*/
//#define NC_HAS_RVALUE_REFS

//pthreadを使うか？
#define NC_USING_PTHREAD

/*
例外対応を行うか
*/
#define NC_NO_EXCEPTIONS

//memo-------

/*
MSVC_VER
_MSC_VER

C Compiler 6.0 600
C/C++ Compiler 7.0 700
Visual C++ 1.0 800
Visual C++ 2.0 900
Visual C++ 4.0 1000
Visual C++ 4.1 1010
Visual C++ 4.2 1020
Visual C++ 5.0 (Visual Studio 97) 1100
Visual C++ 6.0 (Visual Studio 6.0) 1200
Visual C++ 7.0 (Visual Studio.NET 2002) 1300
Visual C++ 7.1 (Visual Studio.NET 2003) 1310
Visual C++ 8.0 (Visual Studio 2005) 1400
Visual C++ 9.0 (Visual Studio 2008) 1500
Visual C++ 10.0 (Visual Studio 2010) 1600
*/


#endif