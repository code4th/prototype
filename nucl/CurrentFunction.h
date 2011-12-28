#if !defined(NC_CURRENT_FUNCTION_H)
#define NC_CURRENT_FUNCTION_H
/*
Copyright (C) 2010 PolygonMagic, Inc. All rights reserved.
*/
#include "NuclConfig.h"

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))

# define NC_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)

# define NC_CURRENT_FUNCTION __FUNCSIG__

#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600))

# define NC_CURRENT_FUNCTION __FUNCTION__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)

# define NC_CURRENT_FUNCTION __func__

#else

# define NC_CURRENT_FUNCTION "(unknown)"

#endif

#endif
