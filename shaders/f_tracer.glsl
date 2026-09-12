#version 460 core

struct triangle {
    vec4 p1;
    vec4 p2;
    vec4 p3;
    vec4 normal;
    uvec4 material_id;
};

struct material {
	vec3 albedo;
	float roughness;
	float metallic;
};

layout(std430, binding = 0) readonly buffer triangle_buffer {
	triangle triangles[];
};

layout(std430, binding = 1) readonly buffer material_buffer {
	material materials[];
};

layout(std430, binding = 2) readonly buffer model_matrix_buffer {
	mat4 models[];
};

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

bool is_inside_trg(vec3 point, vec3 A, vec3 B, vec3 C) {
    vec3 v0 = C - A;
    vec3 v1 = B - A;
    vec3 v2 = point - A;

	float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d02 = dot(v0, v2);
    float d11 = dot(v1, v1);
    float d12 = dot(v1, v2);
	
	float base_den = d00 * d11 - d01 * d01;
	if (abs(base_den) < 1e-8) return false;

	float den = 1.0f / base_den;
	float v = (d11 * d02 - d01 * d12) * den;
	float w = (d00 * d12 - d01 * d02) * den;
	float u = 1.0f - v - w;	
	
	return v >= 0.0f && w >= 0.0f && u >= 0.0f;
}

vec3 get_sky_color(vec3 ray_dir) {
	return vec3((dot(ray_dir, vec3(0, 1, 0)) + 1) / 2, 0.5, 0.5);
}

vec3 test_trace(vec3 ro, vec3 rd) {
	float closest_t = 1e20;
    int hit_idx = -1;

    for (int i = 0; i < triangles.length(); i++) {
		float denom = dot(rd, vec3(triangles[i].normal));
        if (abs(denom) < 1e-6) continue; 

        float t = dot(vec3(triangles[i].p1) - ro, vec3(triangles[i].normal)) / denom;
        if (t < 0.0f || t >= closest_t) continue; 
        
		vec3 plane_point = ro + rd * t;

        if (is_inside_trg(plane_point, vec3(triangles[i].p1), vec3(triangles[i].p2), vec3(triangles[i].p3))) {
            closest_t = t;
            hit_idx = i;
        }
    }

    if (hit_idx != -1) {
        return materials[triangles[hit_idx].material_id.x].albedo * (dot(vec3(triangles[hit_idx].normal), vec3(0, 0, 1) + 1) * 0.5);
    }

    return get_sky_color(rd); 
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

	out_color = vec4(test_trace(camera_pos + vec3(0, 0, 4), ray_dir), 1.0f);
}
