#pragma once

#include "gltypes.h"

#include <stdlib.h>
#include <math.h>


// glmath.c Global //
const float CGUI_ONE_DEG_IN_RAD;
// glmath.c Struct //
// glmath.c Function //
float cgui_deg_to_rad(float degrees);
void cgui_mul_mat4(struct Mat4* mat, struct Mat4* a, struct Mat4* b);
void cgui_perspective
(	struct Mat4* mat,
    float fovy, float aspect,
    float z_near, float z_far);
void cgui_add_vec3f(struct Vec3f* vec, struct Vec3f* a, struct Vec3f* b);
void cgui_sub_vec3f(struct Vec3f* vec, struct Vec3f* a, struct Vec3f* b);
void cgui_normalize_vec3f(struct Vec3f* vec);
void cgui_cross(struct Vec3f* vec, struct Vec3f* a, struct Vec3f* b);
float cgui_dot(struct Vec3f* a, struct Vec3f* b);
void cgui_lookat
(	struct Mat4* mat,
    struct Vec3f* pos_eye,
    struct Vec3f* center_point,
    struct Vec3f* i_up);
void cgui_ortho
(	struct Mat4* mat,
    float left,
    float right,
    float bottom,
    float top,
    float z_near,
    float z_far);

#if defined(__TINYC__) // tcc doesn't play well with headers
	#include "glmath.c"
#endif
