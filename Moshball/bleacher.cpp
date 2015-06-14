/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a bleacher object out of basic shapes.
*/

#include <iostream>
#include "bleacher.h"

using namespace std;
using namespace glm;

//Create a Bleacher object
Bleacher::Bleacher() : Object(){}

Bleacher::~Bleacher(){this->TakeDown();}

//Initialize all the Bleacher elements.
bool Bleacher::Initialize()
{
	if (this->GLReturnedError("Bleacher::Initialize - on entry"))
		return false;

	this->front = new Square3();this->crowd = new Square3();

	vec3 brown = vec3(0.3f, 0.1f, 0.05f);

	this->front->texID = 19;
	this->front->Initialize(1,1.0f, "./textures/wood.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->crowd->texID = 3;
	this->crowd->Initialize(6,1.0f, "./textures/crowd.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	if (this->GLReturnedError("Bleacher::Initialize - on exit"))
		return false;

	return true;
}


void Bleacher::StepShader(){}

//Delete the Bleacher elements.
void Bleacher::TakeDown()
{
	this->crowd = NULL;this->front = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the stool elements.
void Bleacher::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Bleacher::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	mat4 another, scaler;

	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 200.0f;

	//Front of bleachers
	another = modelview;
	another = translate(another, vec3(1.3,0.0,52.8));
	another = rotate(another, 90.0f, vec3(0,0,1));
	scaler = scale(another, vec3(3.5, 1.0, 105.6));
	this->front->Draw(projection, scaler, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));

	another = translate(another, vec3(52.8,0.0,0.0));
	another = rotate(another, 90.0f, vec3(0,0,1));
	scaler = scale(another, vec3(21.0, 1.0, 105.6));
	this->front->Draw(projection, scaler, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-52.8,0.0,0.0));

	another = translate(another, vec3(52.8,20.0,0.0));
	another = rotate(another, -162.0f, vec3(0,0,1));
	scaler = scale(another, vec3(55.6, 1.0, 105.6));
	this->crowd->Draw(projection, scaler, size, 0);
	another = rotate(another, 162.0f, vec3(0,0,1));
	another = translate(another, vec3(-52.8,-20.0,0.0));
	another = translate(another, vec3(-1.3,0.0,-52.8));
	
	if (this->GLReturnedError("Bleacher::Draw - on exit"))
		return;
}
