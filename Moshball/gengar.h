/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Parses and displays a wavefront obj model.
*/

#pragma once
#include "object2.h"
#include "shader2.h"
#include "texturedshader.h"
#include "TextureManager.h"

class Gengar : public Object2
{
public:
	Gengar();
	virtual bool Initialize(const char * objName, const char * jpgName, char* v, char* f);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	char * filename;
	unsigned int texID;

	std::vector<glm::vec4> gengar_vertices;
	std::vector<glm::vec3> gengar_normals;
	std::vector<glm::vec2> gengar_textures;
	std::vector<GLushort> gengar_faces;

private:
	typedef Object2 super;
	int shader_index;

	Shader2 pattern_shader;
	Shader2 basic_shader;
	Shader2 colored_shader;
	Shader2 play_shader;
	TexturedShader basic_texture_shader;
	std::vector<Shader2 *> shaders;
};