#pragma once

#include <string>

unsigned int
compile_shader(unsigned int type, const char* source, const char* label);

unsigned int
link_program(unsigned int vertex_shader, unsigned int fragment_shader);

void report_fatal(const std::string& msg);
