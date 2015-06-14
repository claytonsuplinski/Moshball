/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a shark object out of basic shapes.
*/

#include <iostream>
#include "shark.h"

using namespace std;
using namespace glm;

//Create a Shark object
Shark::Shark() : Object(){}

Shark::~Shark(){this->TakeDown();}

//Initialize all the Shark elements.
bool Shark::Initialize()
{
	if (this->GLReturnedError("Shark::Initialize - on entry"))
		return false;

	this->tooth = new Cylinder();this->tail = new Cylinder2();
	this->tailEnd = new Disc();

	this->fin = new Triangle();

	this->bodyMid = new Sphere2();this->bodyEnd = new Sphere2();
	this->eye = new Sphere();this->eyebrow = new Square();

	this->tooth->color = vec3(0.95f, 0.95f, 0.95f);this->tail->texID = 11;
	this->tailEnd->color = vec3(0.35f, 0.35f, 0.35f);
	this->tooth->Initialize(8,0.175f,0.0f,0.055f, "phong.vert", "phong.frag");
	this->tail->Initialize(8,1.0f,0.0f,0.35f,"./textures/sharkTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->tailEnd->Initialize(8, 0.35f, "phong.vert", "phong.frag");

	this->fin->color = vec3(0.35f, 0.35f, 0.35f);
	this->fin->Initialize(1.0f, "phong.vert", "phong.frag");

	this->bodyMid->texID = 11;this->bodyEnd->texID = 11;this->eye->color = vec3(0.0f, 0.0f, 0.0f);
	this->bodyEnd->Initialize(8,1.0f,"./textures/sharkTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->bodyMid->Initialize(8,1.5f,"./textures/sharkTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->eye->Initialize(8,0.15f, "phong.vert", "phong.frag");

	this->eyebrow->color1 = vec3(0.0f, 0.0f, 0.0f);this->eyebrow->color2 = vec3(0.0f, 0.0f, 0.0f);
	this->eyebrow->Initialize(1,1.0f, "phong.vert", "phong.frag");

	if (this->GLReturnedError("Shark::Initialize - on exit"))
		return false;

	return true;
}


void Shark::StepShader(){
}

//Delete the Shark elements.
void Shark::TakeDown()
{
	this->tooth = NULL;
	this->tail = NULL;
	this->tailEnd = NULL;
	this->fin = NULL;
	this->bodyMid = NULL;
	this->bodyEnd = NULL;
	this->eye = NULL;
	this->eyebrow = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	super::TakeDown();	
}

//Draw and position the Shark elements.
void Shark::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Shark::Draw - on entry"))
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

	//Teeth
	another = translate(another, vec3(0.45,-1.5,-0.25));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.45,1.5,0.25));
	another = translate(another, vec3(0.45,-1.5,0.25));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.45,1.5,-0.25));

	another = translate(another, vec3(0.425,-1.625,-0.18));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.425,1.625,0.18));
	another = translate(another, vec3(0.425,-1.625,0.18));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.425,1.625,-0.18));

	another = translate(another, vec3(0.405,-1.75,-0.13));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.405,1.75,0.13));
	another = translate(another, vec3(0.405,-1.75,0.13));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.405,1.75,-0.13));

	another = translate(another, vec3(0.375,-1.85,-0.05));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.375,1.85,0.05));
	another = translate(another, vec3(0.375,-1.85,0.05));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->tooth->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = translate(another, vec3(-0.375,1.85,-0.05));

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
	
	if (this->GLReturnedError("Shark::Draw - on exit"))
		return;
}
