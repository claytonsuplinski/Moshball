/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a bleacher object out of basic shapes.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "square3.h"
#include "triangle.h"

class Bleacher : public Object
{
public:
	Bleacher();
	~Bleacher();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	Triangle * sideWall;

	Square3 * front;
	Square3 * crowd;
	

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};