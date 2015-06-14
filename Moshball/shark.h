/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a shark object out of basic shapes.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "cylinder2.h"
#include "disc.h"
#include "sphere.h"
#include "sphere2.h"
#include "square.h"
#include "triangle.h"

class Shark : public Object
{
public:
	Shark();
	~Shark();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;

	Cylinder * tooth;Cylinder2 * tail;Disc * tailEnd;

	Triangle * fin;

	Sphere2 * bodyMid;Sphere2 * bodyEnd;Sphere * eye;Square * eyebrow;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};