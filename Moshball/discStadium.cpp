/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates the UFO arena
*/

#include <iostream>
#include "discStadium.h"

using namespace std;
using namespace glm;

//Create the arena
DiscStadium::DiscStadium() : Object()
{
}

DiscStadium::~DiscStadium(){
	this->TakeDown();
}

//Initialize all the elements of the arena
bool DiscStadium::Initialize()
{
	if (this->GLReturnedError("DiscStadium::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->floor = new Square2();this->top = new UfoTop();
	this->triLight = new Triangle();this->triLight2 = new Triangle();
	
	this->floor->texID = 1;
	this->floor->Initialize(120.0f, 10,"./textures/ufoFloor.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->triLight->color = vec3(0.95f, 0.95f, 0.05f);
	this->triLight->Initialize(1.0, "phong.vert", "phong.frag");

	this->triLight2->color = vec3(1.0f, 1.0f, 1.0f);
	this->triLight2->Initialize(1.0, "phong.vert", "phong.frag");

	this->top->Initialize();

	if (this->GLReturnedError("DiscStadium::Initialize - on exit"))
		return false;

	return true;
}

void DiscStadium::StepShader(){
	this->floor->StepShader();this->top->StepShader();
	this->triLight->StepShader();this->triLight2->StepShader();
}

//Delete the DiscStadium object
void DiscStadium::TakeDown()
{
	this->floor = NULL;
	this->top = NULL;
	this->triLight = NULL;
	this->triLight2 = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw and position the elements of the DiscStadium
void DiscStadium::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("DiscStadium::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 another, scaler;
	ivec2 light = ivec2(0,0);

	another = translate(modelview, vec3(-55.0f,0,-55.0f));
	another = rotate(another, 90.0f, vec3(1,0,0));
	this->floor->Draw(projection, another, size, 0);	
	this->top->Draw(projection, modelview, size, 0);

	another = modelview;
	int t = int((glutGet(GLUT_ELAPSED_TIME)) / 800.0f);

	//Flashing floor lights
	if(t%2 == 0){
		for(float i=0; i<360; i+=45){
			another = rotate(another, i, vec3(0,1,0));
			another = translate(another, vec3(7.0,0.1,0.0));
			scaler = scale(another, vec3(49.0f, 1.0f, 7.0f));
			if(i == 90 || i == 180 || i == 270 || i == 0){
				this->triLight->Draw(projection, scaler, light, 180);
			}
			else{this->triLight2->Draw(projection, scaler, light, 180);}
			another = translate(another, vec3(-7.0,-0.1,0.0));
			another = rotate(another, -i, vec3(0,1,0));
		}
	}
	else{
		for(float i=0; i<360; i+=45){
			another = rotate(another, i, vec3(0,1,0));
			another = translate(another, vec3(7.0,0.1,0.0));
			scaler = scale(another, vec3(49.0f, 1.0f, 7.0f));
			if(i == 90 || i == 180 || i == 270 || i == 0){
				this->triLight2->Draw(projection, scaler, light, 180);
			}
			else{this->triLight->Draw(projection, scaler, light, 180);}
			another = translate(another, vec3(-7.0,-0.1,0.0));
			another = rotate(another, -i, vec3(0,1,0));
		}
	}

	if (this->GLReturnedError("DiscStadium::Draw - on exit"))
		return;
}
