#pragma once

#include "gltypes.h"
#include "glmath.h"


#if defined(__TINYC__) // tcc doesn't play well with headers
	#include "gltypes.c"
	#include "glmath.c"
#endif
