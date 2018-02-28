#ifndef _3DMATH_HELPER_H__
#define _3DMATH_HELPER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float w;
    float x;
    float y;
    float z;
}quaternion;

typedef struct {
    float x;
    float y;
    float z;
}vec3f;

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
}vec3l;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
}vec3s;

typedef struct {
    float m[3][3];
}dcm;

#define deg2rad(X)  ((X)*(3.141592653589793/180.0))
#define rad2deg(X)  ((X)*(180.0/3.141592653589793))

static inline vec3s vec3s_zeros() {vec3s c;c.x=c.y=c.z=0.0;return c;}
static inline vec3f vec3f_zeros() {vec3f c;c.x=c.y=c.z=0.0;return c;}

static inline vec3f vec3f_add_vec3f(vec3f a,vec3f b) {vec3f c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;return c;}
static inline vec3f vec3f_add_vec3s(vec3f a,vec3s b) {vec3f c;c.x=a.x+b.x;c.y=a.y+b.y;c.z=a.z+b.z;return c;}

static inline vec3s vec3s_div_s(vec3s a,float b) {vec3s c;c.x=(short)(a.x/b);c.y=(short)(a.y/b);c.z=(short)(a.z/b);return c;}
static inline vec3f vec3f_div_s(vec3f a,float b) {vec3f c;c.x=a.x/b;c.y=a.y/b;c.z=a.z/b;return c;}
static inline vec3f vec3f_div(vec3f a,vec3f b) {vec3f c;c.x=a.x/b.x;c.y=a.y/b.y;c.z=a.z/b.z;return c;}

#ifdef __cplusplus
}
#endif

#endif
