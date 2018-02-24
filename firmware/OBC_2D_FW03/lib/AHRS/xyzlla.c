#include "xyzlla.h"
#include "math.h"

//precision problem ? must use type 'double'
vec3l xyz2lla(vec3l ecef)
{
    /* Variables */
    double b, ep, p, th, lon, lat, n, alt;
    
    /* Constants (WGS ellipsoid) */
    const double a = 6378137;
    const double e = 8.1819190842622e-2;
    const double pi = 3.141592653589793;
    
    /* Calculation */
    b = sqrt(pow(a,2)*(1-pow(e,2)));
    ep = sqrt((pow(a,2)-pow(b,2))/pow(b,2));
    p = sqrt(ecef.x*ecef.x*1e-4+ecef.y*ecef.y*1e-4);
    th = atan2(a*ecef.z*1e-2, b*p);
    lon = atan2(ecef.y, ecef.x);
    lat = atan2((ecef.z*1e-2+ep*ep*b*pow(sin(th),3)),
                (p-e*e*a*pow(cos(th),3)));
    n = a/sqrt(1-e*e*pow(sin(lat),2));
    alt = (p/cos(lat)-n)*1.0e3;
    lat = (lat*180.0e7)/pi;
    lon = (lon*180.0e7)/pi;
    
    return (vec3l)
        {(int32_t)(lon),
         (int32_t)(lat),
         (int32_t)(alt)};
}

vec3l lla2xyz(vec3l lla)
{
    /* Variables */
    double x, y, z, n, lon, lat, alt;
    
    /* Constants (WGS ellipsoid) */
    const double a = 6378137.0;
    const double e = 8.1819190842622e-2;
    const double pi = 3.1415926535897932384626433832795;
    
    /* Calculation */
    lon = lla.x*(pi/180.0e7);
    lat = lla.y*(pi/180.0e7);
    alt = lla.z/1.0e3;
    n = a/sqrt((1.0-pow(e,2)*pow(sin(lat),2)));
    x= (n+alt)*cos(lat)*cos(lon)*1e2;
    y= (n+alt)*cos(lat)*sin(lon)*1e2;
    z= (n*(1-pow(e,2))+alt)*sin(lat)*1e2;
    
    return (vec3l){(int32_t)x,(int32_t)y,(int32_t)z};
}
