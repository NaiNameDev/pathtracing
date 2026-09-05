#version 460 core

#define MAX_DEPTH 4
#define SPP 12

out vec4 out_color;

uniform float time;
uniform vec2 resolution;

uniform float fov;
uniform float near;
uniform float far;

uniform vec3 camera_pos;
uniform vec3 camera_look_dir;

vec3 random_reflection(vec3 normal, int seed) {
	return vec3(1.0);
}

mat3 get_hit_point_noraml_color(vec3 origin, vec3 dir) {
	return mat3(1.0);
}

vec3 trace(vec3 origin, vec3 dir, int seed) {
	mat3 pnc = get_hit_point_noraml_color(origin, dir);
	vec3 color_sum = pnc[0];

	for (int i = 0; i < MAX_DEPTH; i++) {
		pnc = get_dir_pnc(pnc[2], random_reflection(pnc[1], seed + i));
		color_sum *= pnc[0];
	}

	return color_sum;
}

void main() {
	vec3 nlook_dir = normalize(camera_look_dir);

	float aspect = resolution.x / resolution.y;
	float fov_half_tan = tan(fov * 0.5);
	vec2 pix = vec2(((gl_FragCoord.x / resolution.x) - 0.5) * 2.0 * aspect,
				    ((gl_FragCoord.y / resolution.y) - 0.5) * 2.0);

	vec3 right = normalize(cross(nlook_dir, vec3(0, 1, 0)));
	vec3 up = normalize(cross(right, nlook_dir));
	
	vec3 ray_dir = normalize(right * pix.x * fov_half_tan + up * pix.y * fov_half_tan + nlook_dir);

	vec3 color_sum = vec3(0.0);
	for (int i = 0; i < SPP; i++) {
		color_sum += trace(camera_pos, ray_dir, i + int(u_time));
	}

	out_color = color_sum / SPP;
}
