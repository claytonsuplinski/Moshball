/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Creates a stool object out of cylinders, discs, a torus, and stool legs.
*/

#include <iostream>
#include "fish.h"

using namespace std;
using namespace glm;

//Create a stool object
Fish::Fish() : Object()
{
}

Fish::~Fish()
{
	this->TakeDown();
}

//Initialize all the stool elements.
bool Fish::Initialize(vec3 color)
{
	if (this->GLReturnedError("Fish::Initialize - on entry"))
		return false;

	this->tail = new Cylinder();
	this->tailEnd = new Disc();

	this->fin = new Triangle();

	this->bodyMid = new Sphere();
	this->bodyEnd = new Sphere();
	this->eye = new Sphere();

	this->tail->color = color;
	this->tailEnd->color = color;
	this->tail->Initialize(8,1.0f,0.0f,0.35f, "phong.vert", "phong.frag");
	this->tailEnd->Initialize(8, 0.35f, "phong.vert", "phong.frag");

	this->fin->color = color;
	this->fin->Initialize(1.0f, "phong.vert", "phong.frag");

	this->bodyMid->color = color;
	this->bodyEnd->color = color;
	this->eye->color = vec3(0.0f, 0.0f, 0.0f);
	this->bodyEnd->Initialize(8,1.0f, "phong.vert", "phong.frag");
	this->bodyMid->Initialize(8,1.5f, "phong.vert", "phong.frag");
	this->eye->Initialize(8,0.15f, "phong.vert", "phong.frag");

	if (this->GLReturnedError("Fish::Initialize - on exit"))
		return false;

	return true;
}


void Fish::StepShader(){
}

//Delete the stool elements.
void Fish::TakeDown()
{
	this->tail = NULL;
	this->tailEnd = NULL;
	this->fin = NULL;
	this->bodyMid = NULL;
	this->bodyEnd = NULL;
	this->eye = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the stool elements.
void Fish::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Fish::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	mat4 another, scaler;

	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 200.0f;

	//Front of body
	another = modelview;
	scaler = scale(another, vec3(0.65, 2.0, 0.65));
	this->bodyEnd->Draw(projection, scaler, size, 0);

	//Eye
	another = translate(another, vec3(0.0,-1.0,-0.55));
	this->eye->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0,1.0,0.55));

	//Eye
	another = translate(another, vec3(0.0,-1.0,0.55));
	this->eye->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0,1.0,-0.55));

	//Middle of body
	another = rotate(another, 10*sin(t), vec3(1,0,0));
	another = translate(another, vec3(0.0,1.25,0.0));
	scaler = scale(another, vec3(1.0, 2.0, 0.95));
	this->bodyEnd->Draw(projection, scaler, size, 0);

	//Top fin
	another = translate(another, vec3(-2.0,0.0,0.0));
	another = rotate(another, -90.0f, vec3(1,0,0));
	scaler = scale(another, vec3(2.0, 0.65, 2.0));
	this->fin->Draw(projection, scaler, size, 0);
	another = rotate(another, 90.0f, vec3(1,0,0));
	another = translate(another, vec3(2.0,0.0,0.0));

	//Side fin
	another = translate(another, vec3(0.0,-1.0,-0.75));
	another = rotate(another, 90.0f, vec3(0,0,1));
	scaler = scale(another, vec3(1.4, 0.45, 2.25));
	this->fin->Draw(projection, scaler, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(0.0,1.0,0.75));

	//Side fin
	another = translate(another, vec3(0.0,-1.0,0.75));
	another = rotate(another, 90.0f, vec3(0,0,1));
	another = rotate(another, 180.0f, vec3(1,0,0));
	scaler = scale(another, vec3(1.4, 0.45, 2.25));
	this->fin->Draw(projection, scaler, size, 0);
	another = rotate(another, 180.0f, vec3(1,0,0));
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(0.0,1.0,-0.75));

	another = translate(another, vec3(0.0,-1.25,0.0));
	another = rotate(another, -10*sin(t), vec3(1,0,0));

	//Rear of body
	another = rotate(another, 10*sin(t), vec3(1,0,0));
	another = translate(another, vec3(0.0,2.0,0.0));
	scaler = scale(another, vec3(0.45, 2.0, 0.45));
	this->bodyEnd->Draw(projection, scaler, size, 0);

	another = translate(another, vec3(0.0,1.25,0.0));
	scaler = scale(another, vec3(2.0, 2.0, 0.25));
	this->tail->Draw(projection, scaler, size, 0);
	another = translate(another, vec3(0.0,-1.25,0.0));

	another = translate(another, vec3(0.0,-2.0,0.0));
	another = rotate(another, -10*sin(t), vec3(1,0,0));
	
	if (this->GLReturnedError("Fish::Draw - on exit"))
		return;
}
