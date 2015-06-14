/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a square object
*/
#pragma once
#include "object.h"
#include "shader.h"


class Square : public Object
{
public:
	Square();
	virtual bool Initialize(int x, float s, char* v, char* f);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	Shader shader;Shader solid_color;
	glm::vec3 color1, color2;

	int grassFirst;
	glm::mat4 origMVP;glm::mat3 origNM;

	int shader_index;Shader flat_shader;Shader gouraud_shader;Shader phong_shader;
	std::vector<Shader *> shaders;
	inline void StepShader() {this->shader_index = ++this->shader_index % this->shaders.size();};

	GLuint loadBMP_custom(const char * imagepath);
	bool grassShader;

private:
	void BuildNormalVisualizationGeometry();
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	std::vector<VertexAttributesPCNT> verticesT;
	typedef Object super;
};