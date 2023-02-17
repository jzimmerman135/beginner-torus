#ifndef HELPER_INCLUDED
#define HELPER_INCLUDED
#include "vec.h"
#include <curses.h>
#include <time.h>
#include <math.h>

/*
 * uv = pixel coordinate normalized (-0.5, 0.5)
 * p = ray origin position
 * l = ray look at position
 * z = zoom
 * returns the normalized ray direction
 */
float3 get_ray_direction(float2 uv, float3 origin, float3 lookat_pt, float z)
{
    float3 f = normalize3(subtract3(lookat_pt, origin));
    float3 r = normalize3(cross3(f3new(0, 1, 0), f));
    float3 u = cross3(f, r);
    float3 c = scale3(f, z);
    r = scale3(r, uv.x / 2.0);
    u = scale3(u, uv.y / 2.0);
    float3 rd = {c.x + r.x + u.x, c.y + r.y + u.y, c.z + r.z + u.z};
    return normalize3(rd);
}

float torus_SDF(float3 p, float r1, float r2)
{  
    return length2(length2(p.x, p.z) - r1, p.y) - r2;
}

/*
 * p = current position
 * r = radius
 * returns distance to the cube
 */
float cube_SDF(float3 p, float r)
{
    float3 q = {fabsf(p.x) - r, fabsf(p.y) - r, fabsf(p.z) - r};
    float3 m = {fmaxf(q.x, 0.0), fmaxf(q.y, 0.0), fmaxf(q.z, 0.0)};
    float n = fminf(fmaxf(fmaxf(q.x, q.y), q.z), 0.0);
    return length3(m.x, m.y, m.z) + n;
}

/*
 * p = current position
 * r = radius
 * returns distance to the sphere
 */
float sphere_SDF(float3 p, float r)
{
    return length3(p.x, p.y, p.z) - r;
}

#endif