#include <engine/mesh.hpp>

void ssbo_from_mesh_set(std::vector<Mesh*>& scene) {
	std::vector<gpu_triangle> combined_triangle_buffer;
	std::vector<brdf_material> combined_material_buffer;
	std::vector<glm::mat4> combined_model_buffer;
	
	for (unsigned int i = 0; i < scene.size(); i++) {
		for (unsigned int j = 0; j < scene[i]->triangles.size(); j++) {
			combined_triangle_buffer.push_back((gpu_triangle){scene[i]->triangles[j].points[0],
															  scene[i]->triangles[j].points[1],
															  scene[i]->triangles[j].points[2],
															  scene[i]->triangles[j].normal, i});
		}
		combined_material_buffer.push_back(scene[i]->material);
		combined_model_buffer.push_back(scene[i]->get_model());
	}

	unsigned int ssbo[3];
	glGenBuffers(3, ssbo);
	
	// triangle buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, combined_triangle_buffer.size() * sizeof(gpu_triangle), combined_triangle_buffer.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
	// material buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, combined_material_buffer.size() * sizeof(brdf_material), combined_material_buffer.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 1);
	
	// model buffer
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, combined_model_buffer.size() * sizeof(glm::mat4), combined_model_buffer.data(), GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo[2]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 2);
}

glm::mat4 Mesh::get_model() {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	model *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);

	return model;
}

void Mesh::load_from_obj(std::string path) {
	triangles.clear();

	std::vector<float> tv = {}; //vertexes
	std::vector<float> tn = {}; //noramls

	char mod = 'S'; // S = start
	std::vector<std::string> key_words = {"o", "f", "vn", "vt", "v", "usemtl", "s", "#", "g"};
	std::string key_words_mod = "ofntvm   ";
	std::string word = "";

	std::ifstream t(path);
	t.seekg(0, std::ios::end);
	size_t size = t.tellg();
	std::string file(size, ' ');
	t.seekg(0);
	t.read(&file[0], size); 

	int triangle_idx = 0;
	triangle tmp_trg;

	for(int i = 0; i < file.length(); i++) {
		if (file[i] == ' ' || file[i] == '\n') {
			if (word != "") {
				int sk = 0;
				for (int j = 0; j < key_words.size(); j++) {
					if (word == key_words[j]) {
						mod = key_words_mod[j];
						sk = 1;
					}
				}
				
				if (sk == 0) {
					switch(mod) {
						case 'm':
							break;
						case 'o':
							break;
						case 'v':
							tv.push_back(std::stof(word));
							break;
						case 'n':
							tn.push_back(std::stof(word));
							break;
						case 't':
							break;
						case 'f':
							std::vector<int> tmp_nums = {};
							std::string buffer = "";

							for (char ch : word) {
								if (ch != '/') {buffer.push_back(ch); continue;}
								tmp_nums.push_back(std::stoi(buffer)); buffer = "";
							}
							tmp_nums.push_back(std::stoi(buffer));
							
							int overload_idx = (triangle_idx + 1) % 3;
							tmp_trg.points[overload_idx].x = (tv[tmp_nums[0] * 3 - 3]);
							tmp_trg.points[overload_idx].y = (tv[tmp_nums[0] * 3 - 2]);
							tmp_trg.points[overload_idx].z = (tv[tmp_nums[0] * 3 - 1]);

							if (overload_idx == 2) {
								tmp_trg.normal.x = (tn[tmp_nums[2] * 3 - 3]);
								tmp_trg.normal.y = (tn[tmp_nums[2] * 3 - 2]);
								tmp_trg.normal.z = (tn[tmp_nums[2] * 3 - 1]);
								
								triangles.push_back(tmp_trg);
							}
							
							triangle_idx++;
							break;
					}
				}
				word = "";
			}
		}
		else word.push_back(file[i]);
	}
}
