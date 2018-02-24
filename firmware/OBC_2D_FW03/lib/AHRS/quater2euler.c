#include "quater2euler.h"
#include "math.h"

// Converts a quaternion orientation to ZYX Euler angles where phi is a
// rotation around X, theta around Y and psi around Z.
// [X Y Z] = [roll pitch yaw]
vec3f quater2euler(quaternion q)
{
    vec3f eulv;
    
    float r11 = 2* q.w*q.w -1 +2*q.x*q.x;
    float r21 = 2*(q.x*q.y    -  q.w*q.z);
    float r31 = 2*(q.x*q.z    +  q.w*q.y);
    float r32 = 2*(q.y*q.z    -  q.w*q.x);
    float r33 = 2* q.w*q.w -1 +2*q.z*q.z;
    
    eulv.x = atan2(r32, r33 ); // rotate around 
    eulv.y = -atan(r31 / sqrt(1-r31*r31) );
    eulv.z = atan2(r21, r11 );
    
    return eulv;
}

quaternion euler2quater(vec3f a)
{
    float cy,sy,cp,sp,cr,sr;
    
    cy = cos(a.z/2.0F);
    sy = sin(a.z/2.0F);
    cp = cos(a.y/2.0F);
    sp = sin(a.y/2.0F);
    cr = cos(a.x/2.0F);
    sr = sin(a.x/2.0F);
    
    float cycp,cysp,sycp,sysp;
    
    cycp = cy*cp;
    cysp = cy*sp;
    sycp = sy*cp;
    sysp = sy*sp;
    
    quaternion q;
    
    q.w = cycp*cr + sysp*sr;
    q.x = cycp*sr - sysp*cr;
    q.y = cysp*cr + sycp*sr;
    q.z = sycp*cr - cysp*sr;
    
    return q;
}


