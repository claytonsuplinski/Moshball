/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a bird object out of basic shapes.
*/

#include <iostream>
#include "bird.h"

using namespace std;
using namespace glm;

//Create a bird object
Bird::Bird() : Object(){}

Bird::~Bird()
{
	this->TakeDown();
}

//Initialize all the Bird elements.
bool Bird::Initialize(vec3 beakColor, vec3 bodyColor, vec3 tailColor, vec3 wingColor)
{
	if (this->GLReturnedError("Bird::Initialize - on entry"))
		return false;

	this->beak = new Cylinder();this->tail = new Cylinder();

	this->wing = new Triangle();

	this->bodyMid = new Sphere();this->head = new Sphere();
	this->eye = new Sphere();

	this->beak->color = beakColor;this->tail->color = tailColor;
	this->beak->Initialize(8,0.575f,0.0f,0.355f, "phong.vert", "phong.frag");
	this->tail->Initialize(8,1.0f,0.0f,0.35f, "phong.vert", "phong.frag");

	this->wing->color = wingColor;
	this->wing->Initialize(1.0f, "phong.vert", "phong.frag");

	this->bodyMid->color = bodyColor;this->head->color = bodyColor;
	this->eye->color = vec3(0.0f, 0.0f, 0.0f);
	this->head->Initialize(8,1.0f, "phong.vert", "phong.frag");
	this->bodyMid->Initialize(8,1.5f, "phong.vert", "phong.frag");
	this->eye->Initialize(8,0.1f, "phong.vert", "phong.frag");

	if (this->GLReturnedError("Bird::Initialize - on exit"))
		return false;

	return true;
}


void Bird::StepShader(){
}

//Delete the Bird elements.
void Bird::TakeDown(){
	this->beak = NULL;this->tail = NULL;this->head = NULL;
	this->wing = NULL;this->bodyMid = NULL;this->eye = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the Bird elements.
void Bird::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Bird::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	mat4 another, scaler;

	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 200.0f;

	//Front of body
	another = modelview;
	another = translate(another, vec3(-0.5,0.0,0.0));
	scaler = scale(another, vec3(0.65, 1.0, 0.65));
	this->head->Draw(projection, scaler, size, 0);

	//Eye
	another = translate(another, vec3(-0.3,-0.75,-0.35));
	this->eye->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.3,0.75,0.35));

	another = translate(another, vec3(0.0,-1.35,0.0));
	this->beak->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0,1.35,0.0));

	//Eye
	another = translate(another, vec3(-0.3,-0.75,0.35));
	this->eye->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.3,0.75,-0.35));

	//Middle of body
	another = translate(another, vec3(0.0,1.25,0.0));
	scaler = scale(another, vec3(0.75, 1.5, 0.95));
	this->head->Draw(projection, scaler, size, 0);

	//Side fin
	another = rotate(another, 10*sin(t), vec3(0,1,0));
	another = translate(another, vec3(0.0,0.5,-0.75));
	another = rotate(another, -87.0f, vec3(0,0,1));
	scaler = scale(another, vec3(1.3, 0.45, 2.75));
	this->wing->Draw(projection, scaler, size, 0);
	another = rotate(another, 87.0f, vec3(0,0,1));
	another = translate(another, vec3(0.0,-0.5,0.75));
	another = rotate(another, -10*sin(t), vec3(0,1,0));

	//Side fin
	another = rotate(another, -10*sin(t), vec3(0,1,0));
	another = translate(another, vec3(0.0,0.5,0.75));
	another = rotate(another, -87.0f, vec3(0,0,1));
	another = rotate(another, 180.0f, vec3(1,0,0));
	scaler = scale(another, vec3(1.3, 0.45, 2.75));
	this->wing->Draw(projection, scaler, size, 0);
	another = rotate(another, 180.0f, vec3(1,0,0));
	another = rotate(another, 87.0f, vec3(0,0,1));
	another = translate(another, vec3(0.0,-0.5,-0.75));
	another = rotate(another, 10*sin(t), vec3(0,1,0));

	another = translate(another, vec3(0.0,-1.25,0.0));

	//Rear of body
	another = translate(another, vec3(0.0,2.0,0.0));
	scaler = scale(another, vec3(0.45, 2.0, 0.45));
	this->head->Draw(projection, scaler, size, 0);

	another = translate(another, vec3(0.0,1.25,0.0));
	another = rotate(another, 90.0f, vec3(0,1,0));
	scaler = scale(another, vec3(2.0, 2.0, 0.25));
	this->tail->Draw(projection, scaler, size, 0);
	another = rotate(another, -90.0f, vec3(0,1,0));
	another = translate(another, vec3(0.0,-1.25,0.0));

	another = translate(another, vec3(0.0,-2.0,0.0));
	
	if (this->GLReturnedError("Bird::Draw - on exit"))
		return;
}
