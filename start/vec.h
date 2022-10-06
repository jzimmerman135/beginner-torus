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

/* new struct vec2 */
struct vec2 vec2(float x, float y);

struct vec3 {
    float x;
    float y;
    float z;
};

/* new struct vec3 */
struct vec3 vec3(float x, float y, float z);

/* a / length3(a) */
struct vec3 normalize3(struct vec3 a);

/* a * s */
struct vec3 scale3(struct vec3 a, float s);

/* a + b */
struct vec3 add3(struct vec3 a, struct vec3 b);

/* a - b */
struct vec3 subtract3(struct vec3 a, struct vec3 b);

/* dot product */
float dot3(struct vec3 a, struct vec3 b);

/* orthogonal of a and b */
struct vec3 cross3(struct vec3 a, struct vec3 b);

/* rotate struct vec3 around axis by t radians */
struct vec3 rotateX(float3 a, float t);
struct vec3 rotateY(float3 a, float t);
struct vec3 rotateZ(float3 a, float t);

#endif
