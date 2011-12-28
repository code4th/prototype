#ifndef __NC_TLS_h__
#define __NC_TLS_h__

/*
* Define keyword of Thread Local Storage
* スレッド毎に用意された変数を宣言する為のキーワード
* example）
*	NC_THREAD_LOCAL uint32_t name;
*/

#if defined(__GNUC__)

#define NC_THREAD_LOCAL		__thread

#elif defined(_MSC_VER)

#define NC_THREAD_LOCAL		__declspec( thread )

#elif (defined(__INTEL_COMPILER) && defined(_WIN32)) 

#define NC_THREAD_LOCAL		__declspec(thread)

#elif (defined(__INTEL_COMPILER) && defined(__linux__)) 

#define NC_THREAD_LOCAL		__thread

#elif (defined( __SNC__ ))

#define NC_THREAD_LOCAL		__thread

#elif (defined(__MWERKS__) //&& (__MWERKS__ >= 0x3000))

#error I don't know what word is keyword of thread local storage.
#define NC_THREAD_LOCAL		unknown

#else

#error We don't support this compiler.

#endif



#endif // __NC_TLS_h__
