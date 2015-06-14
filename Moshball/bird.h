/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a bird object out of basic shapes.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "sphere.h"
#include "triangle.h"

class Bird : public Object
{
public:
	Bird();
	~Bird();
	virtual bool Initialize(glm::vec3 beakColor, glm::vec3 bodyColor, glm::vec3 tailColor, glm::vec3 wingColor);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	Cylinder * beak;
	Cylinder * tail;

	Triangle * wing;

	Sphere * bodyMid;
	Sphere * head;
	Sphere * eye;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};