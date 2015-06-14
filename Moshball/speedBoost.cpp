/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a speed boost object out of basic shapes.
*/

#include <iostream>
#include "speedBoost.h"

using namespace std;
using namespace glm;

//Create a SpeedBoost object
SpeedBoost::SpeedBoost() : Object()
{
}

SpeedBoost::~SpeedBoost()
{
	this->TakeDown();
}

//Initialize all the SpeedBoost elements.
bool SpeedBoost::Initialize()
{
	if (this->GLReturnedError("SpeedBoost::Initialize - on entry"))
		return false;

	this->tri1 = new Triangle();this->tri2 = new Triangle();this->tri3 = new Triangle();

	this->tri1->color = vec3(1.0f, 0.0f, 0.0f);
	this->tri1->Initialize(2.0f, "phong.vert", "phong.frag");
	this->tri2->color = vec3(1.0f, 1.0f, 1.0f);
	this->tri2->Initialize(2.0f, "phong.vert", "phong.frag");
	this->tri3->color = vec3(1.0f, 1.0f, 0.0f);
	this->tri3->Initialize(2.0f, "phong.vert", "phong.frag");

	if (this->GLReturnedError("SpeedBoost::Initialize - on exit"))
		return false;

	return true;
}


void SpeedBoost::StepShader(){}

//Delete the SpeedBoost elements.
void SpeedBoost::TakeDown()
{
	this->tri1 = NULL;this->tri2 = NULL;this->tri3 = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the SpeedBoost elements.
void SpeedBoost::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("SpeedBoost::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	mat4 another, scaler;

	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	int t2 = (int)(glutGet(GLUT_ELAPSED_TIME) / 100.0f);

	another = modelview;
	another = rotate(another, 90.0f, vec3(1.0f, 0.0f, 0.0f));
	another = rotate(another, t * 300.0f, vec3(0.0f, 0.0f, 1.0f));
	//Change colors on a timer
	if(t2%3 == 0){
		this->tri1->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,0.25,0.0));
		this->tri2->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,0.25,0.0));
		this->tri3->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-0.5,0.0));
	}
	else if(t2%3 == 1){
		this->tri2->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,0.25,0.0));
		this->tri3->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,0.25,0.0));
		this->tri1->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-0.5,0.0));
	}
	else if(t2%3 == 2){
		this->tri3->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,0.25,0.0));
		this->tri1->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,0.25,0.0));
		this->tri2->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-0.5,0.0));
	}


	
	if (this->GLReturnedError("SpeedBoost::Draw - on exit"))
		return;
}
