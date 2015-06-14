/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates an alien object out of cylinders, discs, and spheres.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "cylinder2.h"
#include "disc.h"
#include "sphere.h"
#include "square.h"
#include "sphere2.h"

class Alien : public Object
{
public:
	Alien();
	~Alien();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float rotY);
	void TakeDown();
	void StepShader();
	Shader shader;
	Shader solid_color;
	Cylinder2 * arm;
	Cylinder2 * leg;
	Cylinder * antenna;
	Cylinder2 * body;
	Disc * bodyBot;
	Sphere * center;
	Sphere2 * foot;
	Sphere * eye;
	Sphere2 * head;
	Sphere2 * hand;
	Sphere * mouth;
	Square * eyebrow;
private:
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};