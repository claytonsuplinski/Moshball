/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a speed boost object out of basic shapes.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "triangle.h"

class SpeedBoost : public Object
{
public:
	SpeedBoost();
	~SpeedBoost();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	Triangle * tri1;
	Triangle * tri2;
	Triangle * tri3;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};