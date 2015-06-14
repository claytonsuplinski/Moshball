/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Creates a table object out of cylinders and squares.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "cylinder2.h"
#include "sphere.h"
#include "sphere2.h"

class Tree : public Object
{
public:
	Tree();
	~Tree();
	virtual bool Initialize(int weather);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;
	Shader solid_color;
	Cylinder2 * branch1;
	Cylinder2 * branch2;
	Cylinder2 * branch3;
	Cylinder2 * branch4;
	Cylinder2 * branch5;
	Sphere2 * leaves2;
	Sphere2 * leaves3;
	Sphere2 * leaves4;
	Sphere2 * leaves5;

	int branches2, branches3, branches4, branches5;
	int* branches2X, *branches2Y, *branches3X, *branches3Y;
	int* branches2Z, *branches3Z, *branches4Z, *branches5Z;
	int* branches4X, *branches4Y, *branches5X, *branches5Y;

private:
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};