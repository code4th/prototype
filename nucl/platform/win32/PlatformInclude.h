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

//�g�ݍ��݊�{�^���g��
//#define NC_PLATFORM_USE_INTRINSIC_NATIVE_TYPES


//---------------[ Thread Section ]----------------

//�g�ݍ��݂̃X���b�h���g�p����
//generic�d�l�Ƃ���pthread��p������������܂��B
//pthread���T�|�[�g����Ă�����̏ꍇ�́Ageneric�Ȏ��������p�ł��܂��B
#define NC_PLATFORM_USE_INTRINSIC_THREAD

//�g�ݍ��݂�MUTEX���g�p����
//generic�d�l�Ƃ���pthread��p������������܂��B
//pthread���T�|�[�g����Ă�����̏ꍇ�́Ageneric�Ȏ��������p�ł��܂��B
#define NC_PLATFORM_USE_INTRINSIC_MUTEX

//�g�ݍ��݂̃X���b�h���g�p����igeneric�d�l�������ׁA�K�{�ł��B�j
//#define NC_PLATFORM_USE_INTRINSIC_THREAD

//�g�ݍ��݃A�g�~�b�N�֐�
//#define NC_PLATFORM_USE_INTRINSIC_ATOMIC

//�g�ݍ��݃��j�o�[�T���֐�
#define NC_PLATFORM_USE_INTRINSIC_INTERLOCKED


//---------------[ Thread Section ]----------------

//new�̃I�[�o�[���[�h
#define NC_PLATFORM_USE_INTRINSIC_NEW

//---------------[ Math Section ]----------------

//�g�ݍ���COORDINATE DATA
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
