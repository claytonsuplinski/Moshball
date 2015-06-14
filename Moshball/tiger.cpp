/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a tiger object out of cylinders, discs, and spheres.
*/

#include <iostream>
#include "tiger.h"

using namespace std;
using namespace glm;

//Create a tiger object
Tiger::Tiger() : Object(){}

Tiger::~Tiger(){this->TakeDown();}

//Initialize the elements of the tiger object
bool Tiger::Initialize()
{
	if (this->GLReturnedError("Tiger::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->cO = new Cylinder2();this->cB = new Cylinder2();
	this->leg = new Cylinder2();this->tail = new Cylinder2();
	this->whisker = new Cylinder();

	this->ear = new Disc();

	this->body = new Sphere2();this->eye = new Sphere();this->nose = new Sphere();
	this->head = new Sphere2();this->foot = new Sphere2();

	this->cO->texID = 7;
	this->cO->Initialize(8,0.25f,1.0f,1.0f,"./textures/tigerTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	
	this->cB->texID = 7;
	this->cB->Initialize(8,2.5f,1.0f,1.0f,"./textures/tigerTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->whisker->color = vec3(0.0f, 0.0f, 0.0f);
	this->whisker->Initialize(2,1.0f,0.01f,0.01f, "phong.vert", "phong.frag");

	this->leg->texID = 8;
	this->leg->Initialize(4,2.25f,0.35f,0.5f,"./textures/tigerFace.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");	
	this->tail->texID = 7;
	this->tail->Initialize(4,2.5f,0.05f,0.05f,"./textures/tigerTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");	

	this->ear->color = vec3(0.0f, 0.0f, 0.0f);this->ear->Initialize(8,0.25f, "phong.vert", "phong.frag");

	this->body->texID = 9;
	this->body->Initialize(8,0.98f,"./textures/tigerFace2.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->foot->texID = 8;
	this->foot->Initialize(8,0.65f,"./textures/tigerFace.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	
	this->eye->color = vec3(0.0f, 0.0f, 0.0f);
	this->eye->Initialize(8,0.15f, "phong.vert", "phong.frag");
	this->nose->color = vec3(0.0f, 0.0f, 0.0f);
	this->nose->Initialize(8,0.25f, "phong.vert", "phong.frag");
	this->head->texID = 8;
	this->head->Initialize(8,1.0f,"./textures/tigerFace.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");	

	if (this->GLReturnedError("Tiger::Initialize - on exit"))
		return false;

	return true;
}

void Tiger::StepShader(){
	this->cO->StepShader();this->cB->StepShader();this->leg->StepShader();this->tail->StepShader();
	this->whisker->StepShader();this->ear->StepShader();this->body->StepShader();
	this->eye->StepShader();this->nose->StepShader();this->head->StepShader();
	this->foot->StepShader();
}

//Delete the elements of the tiger
void Tiger::TakeDown()
{
	this->cO = NULL;this->cB = NULL;this->leg = NULL;this->whisker = NULL;
	this->tail = NULL;this->ear = NULL;this->body = NULL;this->eye = NULL;
	this->nose = NULL;this->head = NULL;this->foot = NULL;this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw a tiger object
void Tiger::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Tiger::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 200.0f;

	mat4 another, scaler;
	
	another = rotate(modelview, 40.0f, vec3(0,0,1));
	glLoadMatrixf(glm::value_ptr(another));
	this->tail->Draw(projection, another, size, rotY);
	another = rotate(another, -40.0f, vec3(0,0,1));

	another = rotate(another, 180.0f, vec3(0,0,1));
	another = rotate(another, 180.0f, vec3(0,1,0));
	glLoadMatrixf(glm::value_ptr(another));
	this->body->Draw(projection, another, size, 0);
	this->cB->Draw(projection, another, size, rotY);
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = rotate(another, 90.0f, vec3(1,0,0));
	another = rotate(another, -50.0f, vec3(0,0,1));
	another = rotate(another, abs(35*sin(t)), vec3(1,0,0));
	this->leg->Draw(projection, another, size, rotY);
	another = translate(another, vec3(0.0f, 2.0f, -0.25f));
	scaler = scale(another, vec3(1.0, 1.0, 1.5));
	this->foot->Draw(projection, scaler, size, rotY);
	another = translate(another, vec3(0.0f, -2.0f, 0.25f));
	another = rotate(another, -abs(35*sin(t)), vec3(1,0,0));
	another = rotate(another, 280.0f, vec3(0,0,1));
	another = rotate(another, abs(35*sin(t+45)), vec3(1,0,0));
	this->leg->Draw(projection, another, size, rotY);
	another = translate(another, vec3(0.0f, 2.0f, -0.25f));
	scaler = scale(another, vec3(1.0, 1.0, 1.5));
	this->foot->Draw(projection, scaler, size, rotY);
	another = translate(another, vec3(0.0f, -2.0f, 0.25f));
	another = rotate(another, -abs(35*sin(t+45)), vec3(1,0,0));
	another = rotate(another, -230.0f, vec3(0,0,1));
	another = rotate(another, -90.0f, vec3(1,0,0));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = rotate(another, 90.0f, vec3(1,0,0));
	another = rotate(another, -50.0f, vec3(0,0,1));
	another = rotate(another, -abs(35*sin(t+90)), vec3(1,0,0));
	this->leg->Draw(projection, another, size, rotY);
	another = translate(another, vec3(0.0f, 2.0f, -0.25f));
	scaler = scale(another, vec3(1.0, 1.0, 1.5));
	this->foot->Draw(projection, scaler, size, rotY);
	another = translate(another, vec3(0.0f, -2.0f, 0.25f));
	another = rotate(another, abs(35*sin(t+90)), vec3(1,0,0));
	another = rotate(another, 280.0f, vec3(0,0,1));
	another = rotate(another, -abs(35*sin(t+90+45)), vec3(1,0,0));
	this->leg->Draw(projection, another, size, rotY);
	another = translate(another, vec3(0.0f, 2.0f, -0.25f));
	scaler = scale(another, vec3(1.0, 1.0, 1.5));
	this->foot->Draw(projection, scaler, size, rotY);
	another = translate(another, vec3(0.0f, -2.0f, 0.25f));
	another = rotate(another, abs(35*sin(t+90+45)), vec3(1,0,0));
	another = rotate(another, -230.0f, vec3(0,0,1));
	another = rotate(another, -90.0f, vec3(1,0,0));
	another = translate(another, vec3(0.0f, 0.25f, 0.0f));
	another = rotate(another, 90.0f, vec3(0,0,1));
	this->body->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,0,1));
	another = rotate(another, -180.0f, vec3(0,1,0));
	another = rotate(another, -180.0f, vec3(0,0,1));

	//trans and ang = 0

	another = rotate(another, 40.0f, vec3(0,0,1));
	another = translate(another, vec3(-1.5f, 0.0f, 0.0f));
	another = rotate(another, -40.0f, vec3(0,0,1));
	this->head->Draw(projection, another, size, 0);
	another = rotate(another, 40.0f, vec3(0,0,1));
	another = rotate(another, -40.0f, vec3(0,0,1));

	//trans = back of head; ang = 0;

	another = translate(another, vec3(-1.0f, 0.0f, 0.5f));
	this->ear->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0f, 0.0f, -1.0f));
	this->ear->Draw(projection, another, size, 0);
	another = translate(another, vec3(1.0f, 0.0f, 0.5f));
	another = translate(another, vec3(0.0f, -1.0f, 0.0f));
	this->nose->Draw(projection, another, size, 0);
	another = rotate(another, 90.0f, vec3(1,0,0));
	this->whisker->Draw(projection, another, size, rotY);
	another = rotate(another, 20.0f, vec3(0,0,1));
	this->whisker->Draw(projection, another, size, rotY);
	another = rotate(another, -40.0f, vec3(0,0,1));
	this->whisker->Draw(projection, another, size, rotY);
	another = rotate(another, 200.0f, vec3(0,0,1));
	this->whisker->Draw(projection, another, size, rotY);
	another = rotate(another, 20.0f, vec3(0,0,1));
	this->whisker->Draw(projection, another, size, rotY);
	another = rotate(another, -40.0f, vec3(0,0,1));
	this->whisker->Draw(projection, another, size, rotY);
	another = rotate(another, -160.0f, vec3(0,0,1));
	another = rotate(another, -90.0f, vec3(1,0,0));
	another = translate(another, vec3(0.0f, 1.0f, 0.0f));

	//trans = back of head; ang = 0;

	another = translate(another, vec3(-0.7f, -0.6f, 0.35f));
	this->eye->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0f, 0.0f, -0.7f));
	this->eye->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.7f, 0.6f, 0.35f));

	another = rotate(another, 40.0f, vec3(0,0,1));
	another = translate(another, vec3(1.5f, 0.0f, 0.0f));
	another = rotate(another, -40.0f, vec3(0,0,1));

	//trans and ang = 0

	if (this->GLReturnedError("Tiger::Draw - on exit"))
		return;
}
