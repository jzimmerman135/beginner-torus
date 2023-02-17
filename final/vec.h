#ifndef VEC_INCLUDED
#define VEC_INCLUDED

#ifndef min
#define min(a, b) (a > b ? b : a)
#endif
#ifndef max
#define max(a, b) (a > b ? a : b)
#endif

/* clamp argument a to the within the range [b, c] */
float clamp(float a, float b, float c);

/* linear interpolate between a, b based on t in [0, 1] */
float lerp(float a, float b, float t);

/* 2D euclidean distance, sqrt(x^2 + y^2) */
float length2(float x, float y);

/* 3D euclidean distance, sqrt(x^2 + y^2 + z^2) */
float length3(float x, float y, float z);

typedef struct {
    float x;
    float y;
} float2;

/* new float2 */
float2 f2new(float x, float y);
float2 scale2(float2 a, float s);
float2 add2(float2 a, float2 b);

typedef struct {
    float x;
    float y;
    float z;
} float3;

/* new float3 */
float3 f3new(float x, float y, float z);

/* a / length3(a) */
float3 normalize3(float3 a);

/* a * s */
float3 scale3(float3 a, float s);

/* a + b */
float3 add3(float3 a, float3 b);

/* a - b */
float3 subtract3(float3 a, float3 b);

/* dot product */
float dot3(float3 a, float3 b);

/* orthogonal of a and b */
float3 cross3(float3 a, float3 b);

/* rotate float3 around axis by t radians */
float3 rotateX(float3 a, float t);
float3 rotateY(float3 a, float t);
float3 rotateZ(float3 a, float t);

#endif
