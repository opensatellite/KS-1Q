#ifndef __XYZLLA_H__
#define __XYZLLA_H__

#include "3dmath_helper.h"

/*
vec3l lla = {
    lon * 1e-7deg/LSB,
    lat * 1e-7deg/LSB,
    alt *     1mm/LSB       

vec3l xyz = {
    x, y, z, (1cm/LSB)
}
*/

vec3l xyz2lla(vec3l);
vec3l lla2xyz(vec3l);

#endif