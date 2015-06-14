/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a coin object out of basic shapes.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "cylinder.h"
#include "disc.h"

class Coin : public Object
{
public:
	Coin();
	~Coin();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;
	Cylinder * side;
	Disc * face;
private:
	void BuildNormalVisualizationGeometry();
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};