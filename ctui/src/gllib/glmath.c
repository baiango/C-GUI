const float CGUI_ONE_DEG_IN_RAD = 0.0174533f;
float cgui_deg_to_rad(float degrees) { return degrees * 0.0174533f; }


void cgui_mul_mat4(struct mat4* mat, struct mat4* a, struct mat4* b)
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
(	struct mat4* mat,
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
(	struct mat4* mat,
    struct Vec3f* pos_eye,
    struct Vec3f* center_point,
    struct Vec3f* i_up)
{	struct Vec3f forward;
	cgui_sub_vec3f(&forward, center_point, pos_eye);
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
	mat->data[4 * 3] = -cgui_dot(&right, pos_eye);
	mat->data[4 * 3 + 1] = -cgui_dot(&up, pos_eye);
	mat->data[4 * 3 + 2] = cgui_dot(&forward, pos_eye); }
