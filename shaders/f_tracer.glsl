#version 460 core

#define MAX_DEPTH 50

out vec4 out_color;

uniform float time;
uniform vec2 resolution;

uniform float fov;
uniform float near;
uniform float far;

uniform vec3 camera_pos;
uniform vec3 camera_look_dir;

void main() {
	vec3 nlook_dir = normalize(camera_look_dir);

	float aspect = resolution.x / resolution.y;
	float fov_half_tan = tan(fov * 0.5);
	vec2 pix = vec2(((gl_FragCoord.x / resolution.x) - 0.5) * 2.0 * aspect,
				    ((gl_FragCoord.y / resolution.y) - 0.5) * 2.0);

	vec3 right = normalize(cross(nlook_dir, vec3(0, 1, 0)));
	vec3 up = normalize(cross(right, nlook_dir));
	
	vec3 ray_dir = normalize(right * pix.x * fov_half_tan + up * pix.y * fov_half_tan + nlook_dir);

	out_color = vec4(dot(ray_dir, vec3(0, 0, -1)), 1.0, 1.0, 1.0);
}
