const float CGUI_ONE_DEG_IN_RAD = 0.0174533f;
float cgui_deg_to_rad(float degrees) { return degrees * 0.0174533f; }


void cgui_mul_mat4(struct Mat4* mat, struct Mat4* a, struct Mat4* b)
{	for (size_t i = 0; i < 4; i++)
	{	for (size_t j = 0; j < 4; j++)
		{	mat->data[4 * j + i] = 0.0f;

			for (size_t k = 0; k < 4; k++)
			{	mat->data[4 * j + i] += a->data[4 * k + i] * b->data[4 * j + k]; } } } }

// fovy: Vertical field of view angle in degrees.
// aspect: Aspect ratio of the viewport. It's calculated as width / height.
// z_near: Objects closer than this value will not be visible.
// z_far: Objects farther away than this value will not be visible.
void cgui_perspective
(	struct Mat4* mat,
    float fovy, float aspect,
    float z_near, float z_far)
{	float tan_half_fovy = tan(fovy / 2.0f);

	cgui_zero_mat4(mat);
	mat->data[0] = 1.0f / (aspect * tan_half_fovy);
	mat->data[4 * 1 + 1] = 1.0f / tan_half_fovy;
	mat->data[4 * 2 + 2] = -(z_far + z_near) / (z_far - z_near);
	mat->data[4 * 2 + 3] = -1.0f;
	mat->data[4 * 3 + 2] = -(2.0f * z_far * z_near) / (z_far - z_near); }


void cgui_add_vec3f(struct Vec3f* vec, struct Vec3f* a, struct Vec3f* b)
{	vec->x = a->x + b->x;
	vec->y = a->y + b->y;
	vec->z = a->z + b->z; }


void cgui_sub_vec3f(struct Vec3f* vec, struct Vec3f* a, struct Vec3f* b)
{	vec->x = a->x - b->x;
	vec->y = a->y - b->y;
	vec->z = a->z - b->z; }


void cgui_normalize_vec3f(struct Vec3f* vec)
{	if (0.0f == vec->x + vec->y + vec->z)
	{	printf("cgui_normalize_vec3f: Zeroed vector found! Skipping...");
		return; }

	float length =
	    sqrt(vec->x * vec->x
	        + vec->y * vec->y
	        + vec->z * vec->z);
	float inv_length = 1.0f / length;
	vec->x *= inv_length;
	vec->y *= inv_length;
	vec->z *= inv_length; }


void cgui_cross(struct Vec3f* vec, struct Vec3f* a, struct Vec3f* b)
{	vec->x = a->y * b->z
	    - a->z * b->y;
	vec->y = a->z * b->x
	    - a->x * b->z;
	vec->z = a->x * b->y
	    - a->y * b->x; }


float cgui_dot(struct Vec3f* a, struct Vec3f* b)
{	return a->x * b->x
	    + a->y * b->y
	    + a->z * b->z; }


void cgui_lookat
(	struct Mat4* mat,
    struct Vec3f* pos_camera,
    struct Vec3f* target,
    struct Vec3f* i_up)
{	struct Vec3f forward;
	cgui_sub_vec3f(&forward, target, pos_camera);
	cgui_normalize_vec3f(&forward);

	struct Vec3f right;
	cgui_cross(&right, &forward, i_up);
	cgui_normalize_vec3f(&right);

	struct Vec3f up;
	cgui_cross(&up, &right, &forward);

	cgui_set_diagonal_mat4(mat, 1.0f);
	mat->data[0] = right.x;
	mat->data[4 * 1] = right.y;
	mat->data[4 * 2] = right.z;
	mat->data[1] = up.x;
	mat->data[4 * 1 + 1] = up.y;
	mat->data[4 * 2 + 1] = up.z;
	mat->data[2] = -forward.x;
	mat->data[4 * 1 + 2] = -forward.y;
	mat->data[4 * 2 + 2] = -forward.z;
	mat->data[4 * 3] = -cgui_dot(&right, pos_camera);
	mat->data[4 * 3 + 1] = -cgui_dot(&up, pos_camera);
	mat->data[4 * 3 + 2] = cgui_dot(&forward, pos_camera); }


// Let's assune that we want 1 meter canva from 16:9 monitor.
//	struct Vec3f camera_position;
//	struct Vec3f camera_target;
//	struct Vec3f camera_up;
//	cgui_set_vec3f_from_floats(&camera_position, 0.0f, -0.0f, -1.0f);
//	cgui_set_vec3f_from_floats(&camera_target, 0.0f, 0.0f, 0.0f);
//	cgui_set_vec3f_from_floats(&camera_up, 0.0f, 1.0f, 0.0f);
//
//	struct Mat4 view2;
//	cgui_lookat(&view2, &camera_position, &camera_target, &camera_up);
//	float left = -0.5f; // Half a meter from middle to the left
//	float right = 0.5f; // Half a meter from middle to the right
//	float bottom = -0.28125f; // 16:9 aspect ratio, so -0.5 * 9 / 16 = -0.28125
//	float top = 0.28125f; // 16:9 aspect ratio, so 0.5 * 9 / 16 = 0.28125
//	float near = 0.01f; // Anything closer than this will clip
//	float far = 4000.0f; // Anything further than this will clip
//
//	struct Mat4 ortho_proj;
//	cgui_ortho(&ortho_proj, left, right, bottom, top, near, far);
//
//	struct Mat4 ortho_view_proj;
//	cgui_mul_mat4(&ortho_view_proj, &ortho_proj, &view2);
void cgui_ortho
(	struct Mat4* mat,
    float left,
    float right,
    float bottom,
    float top,
    float z_near,
    float z_far)
{	cgui_zero_mat4(mat);
	cgui_set_diagonal_mat4(mat, 1.0f);
	mat->data[0] = 2.0f / (right - left);
	mat->data[4 * 1 + 1] = 2.0f / (top - bottom);
	mat->data[4 * 2 + 2] = -2.0f / (z_far - z_near);
	mat->data[4 * 3] = -(right + left) / (right - left);
	mat->data[4 * 3 + 1] = -(top + bottom) / (top - bottom);
	mat->data[4 * 3 + 2] = -(z_far + z_near) / (z_far - z_near); }
