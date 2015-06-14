/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a ufo ceiling object out of basic shapes.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "bowl2.h"
#include "sphere.h"
#include "torus.h"

class UfoTop : public Object
{
public:
	UfoTop();
	~UfoTop();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;
	Shader solid_color;
	Bowl2 * ufo;
	Sphere * light;Sphere * light2;Sphere * light3;Sphere * light4;
	Torus * band;

private:
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};