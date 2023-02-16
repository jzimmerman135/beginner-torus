#include "vec.h"
#include "math.h"

float clamp(float a, float b, float c)
{
    return max(b, min(a, c));
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * clamp(t, 0.0, 1.0);
}

/*******************************************************************************
                                2D VECTOR FUNCTIONS 
 ******************************************************************************/

float2 f2new(float x, float y)
{
    return (float2){x, y};
}

float length2(float x, float y)
{
    return sqrt(x * x + y * y);
}

float2 scale2(float2 a, float s)
{
    return (float2){a.x * s, a.y * s};
}

/*******************************************************************************
                                3D VECTOR FUNCTIONS 
 ******************************************************************************/

float3 f3new(float x, float y, float z)
{
    return (float3){x, y, z};
}

float length3(float x, float y, float z)
{
    return sqrt(x * x + y * y + z * z);
}

float3 normalize3(float3 a)
{
    float l = length3(a.x, a.y, a.z);
    return scale3(a, 1.0 / l);
}

float3 add3(float3 a, float3 b) 
{
    return (float3){a.x + b.x, a.y + b.y, a.z + b.z};
}

float3 subtract3(float3 a, float3 b) 
{
    return (float3){a.x - b.x, a.y - b.y, a.z - b.z};
}

float3 scale3(float3 a, float s)
{
    return (float3){a.x * s, a.y * s, a.z * s};
}

float dot3(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 cross3(float3 a, float3 b)
{
    return (float3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float3 rotate3X(float3 a, float t)
{
    return (float3){
        a.x,
        a.z * cos(t) + a.y * sin(t),
        a.y * cos(t) - a.z * sin(t)
    };
}

float3 rotate3Y(float3 a, float t)
{
    return (float3){
        a.x * cos(t) - a.z * sin(t),
        a.y,
        a.z * cos(t) + a.x * sin(t)
    };
}

float3 rotate3Z(float3 a, float t)
{
    return (float3){
        a.x * cos(t) + a.y * sin(t),
        a.y * cos(t) - a.x * sin(t),
        a.z
    };
}