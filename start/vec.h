#ifndef VEC_INCLUDED
#define VEC_INCLUDED
#ifndef min
#define min(a, b) (a > b ? b : a)
#endif
#ifndef max
#define max(a, b) (a > b ? a : b)
#endif

/* return a from range [b, c] */
float clampf(float a, float b, float c);

/* linear interpolate between a, b based on t in [0, 1] */
float lerpf(float a, float b, float t);

/* 2D euclidean distance, sqrt(x^2 + y^2) */
float length2f(float x, float y);

/* 3D euclidean distance, sqrt(x^2 + y^2 + z^2) */
float length3f(float x, float y, float z);

typedef struct vec3 float3;
typedef struct vec2 float2;

struct vec2 {
    float x;
    float y;
};

/* new float2 */
float2 vec2(float x, float y);
float2 scale2(float2 a, float s);

struct vec3 {
    float x;
    float y;
    float z;
};

/* new float3 */
float3 vec3(float x, float y, float z);

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
