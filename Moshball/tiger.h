/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a tiger object out of cylinders, discs, and spheres.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "cylinder2.h"
#include "disc.h"
#include "sphere.h"
#include "sphere2.h"

class Tiger : public Object
{
public:
	Tiger();
	~Tiger();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float rotY);
	void TakeDown();
	void StepShader();
	Shader shader;
	Shader solid_color;
	Cylinder2 * cO;Cylinder2 * cB;Cylinder2 * leg;
	Cylinder2 * tail;Cylinder * whisker;
	Disc * ear;
	Sphere2 * body;Sphere * eye;Sphere * nose;
	Sphere2 * head;Sphere2 * foot;
private:
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};