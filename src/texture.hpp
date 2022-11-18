#include <string>

unsigned int texture_create(const std::string& file_path, const bool is_rgba);
void texture_bind(unsigned int texture_id);
void texture_unbind();
