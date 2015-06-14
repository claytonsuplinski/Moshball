/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Creates a stool object out of cylinders, discs, a torus, and stool legs.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "disc.h"
#include "sphere.h"
#include "square.h"
#include "triangle.h"

class Fish : public Object
{
public:
	Fish();
	~Fish();
	virtual bool Initialize(glm::vec3 color);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	Cylinder * tail;
	Disc * tailEnd;

	Triangle * fin;

	Sphere * bodyMid;
	Sphere * bodyEnd;
	Sphere * eye;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};