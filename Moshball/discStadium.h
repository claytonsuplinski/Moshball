/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates the UFO arena
*/

#pragma once
#include "object.h"
#include "shader.h"
#include "disc.h"
#include "ufoTop.h"
#include "triangle.h"
#include "square2.h"

class DiscStadium : public Object
{
public:
	DiscStadium();
	~DiscStadium();
	virtual bool Initialize();
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	Shader shader;
	Shader solid_color;
	Square2 * floor;
	UfoTop * top;
	Triangle * triLight;
	Triangle * triLight2;
private:
	void BuildNormalVisualizationGeometry();
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};