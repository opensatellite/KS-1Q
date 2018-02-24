#ifndef _QUATER2EULER_H__
#define _QUATER2EULER_H__

#include "3dmath_helper.h"

#ifdef __cplusplus
extern "C" {
#endif
    
vec3f quater2euler(quaternion q);
quaternion euler2quater(vec3f eulv);

#ifdef __cplusplus
}
#endif

#endif
