/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a ufo ceiling object out of basic shapes.
*/

#include <iostream>
#include "ufoTop.h"

using namespace std;
using namespace glm;

//Make the UfoTop object
UfoTop::UfoTop() : Object()
{
}

UfoTop::~UfoTop(){
	this->TakeDown();
}

//Initialize all the elements of the UfoTop
bool UfoTop::Initialize()
{
	if (this->GLReturnedError("UfoTop::Initialize - on entry"))
		return false;

	this->ufo = new Bowl2();this->light = new Sphere();this->light2 = new Sphere();
	this->light3 = new Sphere();this->light4 = new Sphere();this->band = new Torus();

	this->ufo->texID = 2;
	this->ufo->Initialize(8,52.8f, "./textures/ufoRoof.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");	
	this->light->color = vec3(0.0f, 1.0f, 0.0f);
	this->light->Initialize(4,1.25f, "phong2.vert", "phong2.frag");
	this->light2->color = vec3(1.0f, 0.0f, 1.0f);
	this->light2->Initialize(4,1.25f, "phong2.vert", "phong2.frag");
	this->light3->color = vec3(1.0f, 1.0f, 0.0f);
	this->light3->Initialize(4,1.25f, "phong2.vert", "phong2.frag");
	this->light4->color = vec3(1.0f, 0.5f, 1.0f);
	this->light4->Initialize(4,1.25f, "phong2.vert", "phong2.frag");
	this->band->color = vec3(0.95f, 0.95f, 0.15f);
	this->band->Initialize(4,51.0f, 1.5f, "phong2.vert", "phong2.frag");

	if (this->GLReturnedError("UfoTop::Initialize - on exit"))
		return false;

	return true;
}

void UfoTop::StepShader(){
}

//Delete all the elements of the UfoTop
void UfoTop::TakeDown(){
	this->ufo = NULL;this->light = NULL;this->light2 = NULL;
	this->light3 = NULL;this->light4 = NULL;this->band = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw and position the UfoTop elements
void UfoTop::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("UfoTop::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	int t = int((glutGet(GLUT_ELAPSED_TIME)) / 1000.0f);

	mat4 another, scaler;
	
	this->ufo->Draw(projection, modelview, size, 180);
	another = modelview;
	another = translate(another, vec3(0.0,10.0,0.0));
	another = rotate(another, 90.0f, vec3(1,0,0));
	this->band->Draw(projection, another, size, 180);
	another = rotate(another, -90.0f, vec3(1,0,0));
	another = translate(another, vec3(0.0,-10.0,0.0));

	//Position lights & have them blink
	if(t%2 == 0){
		for(float i=0; i<360; i+=60){
			another = translate(another, vec3(0.0,5.0,0.0));
			another = rotate(another, i, vec3(0,1,0));
			another = translate(another, vec3(52.0,0.0,0.0));
			if(i == 120 || i == 300){this->light->Draw(projection, another, size, 180);}
			else if(i == 60){this->light3->Draw(projection, another, size, 180);}
			else if(i == 240){this->light4->Draw(projection, another, size, 180);}
			else{this->light2->Draw(projection, another, size, 180);}
			another = translate(another, vec3(-52.0,0.0,0.0));
			another = rotate(another, -i, vec3(0,1,0));
			another = translate(another, vec3(0.0,-5.0,0.0));
		}
		for(float i=30; i<390; i+=60){
			another = translate(another, vec3(0.0,15.0,0.0));
			another = rotate(another, i, vec3(0,1,0));
			another = translate(another, vec3(49.0,0.0,0.0));
			if(i == 120+30 || i == 300+30){this->light2->Draw(projection, another, size, 180);}
			else if(i == 60+30){this->light4->Draw(projection, another, size, 180);}
			else if(i == 240+30){this->light3->Draw(projection, another, size, 180);}
			else{this->light->Draw(projection, another, size, 180);}
			another = translate(another, vec3(-49.0,0.0,0.0));
			another = rotate(another, -i, vec3(0,1,0));
			another = translate(another, vec3(0.0,-15.0,0.0));
		}
	}
	else{
		for(float i=0; i<360; i+=60){
			another = translate(another, vec3(0.0,5.0,0.0));
			another = rotate(another, i, vec3(0,1,0));
			another = translate(another, vec3(52.0,0.0,0.0));
			if(i == 120 || i == 300){this->light3->Draw(projection, another, size, 180);}
			else if(i == 60){this->light->Draw(projection, another, size, 180);}
			else if(i == 240){this->light2->Draw(projection, another, size, 180);}
			else{this->light4->Draw(projection, another, size, 180);}
			another = translate(another, vec3(-52.0,0.0,0.0));
			another = rotate(another, -i, vec3(0,1,0));
			another = translate(another, vec3(0.0,-5.0,0.0));
		}
		for(float i=30; i<390; i+=60){
			another = translate(another, vec3(0.0,15.0,0.0));
			another = rotate(another, i, vec3(0,1,0));
			another = translate(another, vec3(49.0,0.0,0.0));
			if(i == 120+30 || i == 300+30){this->light4->Draw(projection, another, size, 180);}
			else if(i == 60+30){this->light2->Draw(projection, another, size, 180);}
			else if(i == 240+30){this->light->Draw(projection, another, size, 180);}
			else{this->light3->Draw(projection, another, size, 180);}
			another = translate(another, vec3(-49.0,0.0,0.0));
			another = rotate(another, -i, vec3(0,1,0));
			another = translate(another, vec3(0.0,-15.0,0.0));
		}
	}

	if (this->GLReturnedError("UfoTop::Draw - on exit"))
		return;
}
