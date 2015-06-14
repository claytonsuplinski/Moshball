/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a coin object out of basic shapes.
*/

#include <iostream>
#include "coin.h"

using namespace std;
using namespace glm;

//Create the coin object
Coin::Coin() : Object(){}

Coin::~Coin(){this->TakeDown();}

//Initialize all the elements of the Coin object
bool Coin::Initialize()
{
	if (this->GLReturnedError("Coin::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->side = new Cylinder();
	this->face = new Disc();

	this->face->color = vec3(0.45f, 0.45f, 0.0f);
	this->face->Initialize(18, 1.0f, "phong.vert", "phong.frag");
	
	this->side->color = vec3(0.45f, 0.45f, 0.0f);
	this->side->Initialize(18,0.25f, 1.0f, 1.0f, "phong.vert", "phong.frag");
	
	if (this->GLReturnedError("Coin::Initialize - on exit"))
		return false;

	return true;
}

void Coin::StepShader(){this->face->StepShader();this->side->StepShader();}

//Delete the Coin object
void Coin::TakeDown()
{
	this->face = NULL;this->side = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();
}

//Draw and position the elements of the Coin
void Coin::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Coin::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	mat4 another;

	modelview = rotate(modelview, 90.0f, vec3(1.0f, 0.0f, 0.0f));
	modelview = rotate(modelview, t * 300.0f, vec3(0.0f, 0.0f, 1.0f));
	
	this->side->Draw(projection, modelview, size, 0);

	this->face->Draw(projection, modelview, size, 0);

	another = translate(modelview, vec3(0.0f,0.25f,0.0f));
	glLoadMatrixf(glm::value_ptr(another));
	this->face->Draw(projection, another, size, 0);

	if (this->GLReturnedError("Coin::Draw - on exit"))
		return;
}
