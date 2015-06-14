/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a jumbotron object out of basic shapes.
*/


#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "square4.h"

class Jumbotron : public Object
{
public:
	Jumbotron();
	~Jumbotron();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	Cylinder * base;
	Square4 * screen;
	GLuint fboID;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};