/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates an alien object out of cylinders, discs, and spheres.
*/

#include <iostream>
#include "alien.h"

using namespace std;
using namespace glm;

//Create a alien object
Alien::Alien() : Object(){}

Alien::~Alien(){
	this->TakeDown();
}

//Initialize the elements of the Alien object
bool Alien::Initialize()
{
	if (this->GLReturnedError("Alien::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->arm = new Cylinder2();this->antenna = new Cylinder();
	this->leg = new Cylinder2();this->body = new Cylinder2();

	this->bodyBot = new Disc();

	this->center = new Sphere();this->eye = new Sphere();
	this->mouth = new Sphere();	this->head = new Sphere2();
	this->hand = new Sphere2();	this->foot = new Sphere2();
	this->center->lava = true;

	this->eyebrow = new Square();

	this->arm->texID = 4;
	this->arm->Initialize(8,2.0f,0.15f,0.15f,"./textures/alienTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	
	this->body->texID = 10;
	this->body->Initialize(10,1.25f,1.25f,0.4f,"./textures/clothTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->antenna->color = vec3(0.0f, 0.0f, 0.0f);
	this->antenna->Initialize(2,2.5f,0.05f,0.05f, "phong.vert", "phong.frag");

	this->leg->texID = 4;
	this->leg->Initialize(8,0.85f,0.2f,0.2f,"./textures/alienTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");	

	this->bodyBot->color = vec3(0.35f, 0.3f, 0.25f);
	this->bodyBot->Initialize(10,1.25f, "phong.vert", "phong.frag");

	this->center->color = vec3(1.0f, 0.15f, 0.05f);
	this->center->Initialize(8,0.35f, "phong.vert", "phong.frag");

	this->foot->texID = 4;
	this->foot->Initialize(8,0.25f,"./textures/alienTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	
	this->eye->color = vec3(0.0f, 0.0f, 0.0f);
	this->eye->Initialize(8,0.75f, "phong.vert", "phong.frag");	

	this->head->texID = 4;
	this->head->Initialize(2,2.0f,"./textures/alienTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");	

	this->hand->texID = 4;
	this->hand->Initialize(8,0.35f, "./textures/alienTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->mouth->color = vec3(0.0f, 0.0f, 0.0f);this->mouth->Initialize(8,0.15f, "phong.vert", "phong.frag");

	this->eyebrow->color1 = vec3(0.0f, 0.0f, 0.0f);this->eyebrow->color2 = vec3(0.0f, 0.0f, 0.0f);
	this->eyebrow->Initialize(1,1.0f, "phong.vert", "phong.frag");

	if (this->GLReturnedError("Tiger::Initialize - on exit"))
		return false;

	return true;
}

void Alien::StepShader(){
	
}

//Delete the elements of the Alien
void Alien::TakeDown(){
	this->arm = NULL;this->antenna = NULL;this->leg = NULL;
	this->bodyBot = NULL;this->center = NULL;this->hand = NULL;
	this->body = NULL;this->eye = NULL;this->mouth = NULL;
	this->head = NULL;this->foot = NULL;this->eyebrow = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw a Alien object
void Alien::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float rotY)
{
	if (this->GLReturnedError("Alien::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	float t = float(glutGet(GLUT_ELAPSED_TIME)) / 150.0f;
	mat4 another, scaler;

	//Make it run
	if(rotY == 0){	
		another = translate(modelview, vec3(0.35,0.0,0));
		this->center->Draw(projection, another, size, rotY);
		another = translate(another, vec3(-0.35,0.0,0));

		another = translate(another, vec3(0.85,0.95,0));
		another = rotate(another, 90.0f, vec3(0,0,1));
		this->body->Draw(projection, another, size, rotY);
		this->bodyBot->Draw(projection, another, size, rotY);

		//Arms
		another = translate(another, vec3(0.0,0.35,0.35));
		another = rotate(another, 45.0f, vec3(1,0,0));
		this->arm->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,1.75,0.0));
		this->hand->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-1.75,0.0));
		another = rotate(another, -45.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.35,-0.35));

		another = translate(another, vec3(0.0,0.35,-0.35));
		another = rotate(another, -45.0f, vec3(1,0,0));
		this->arm->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,1.75,0.0));
		this->hand->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-1.75,0.0));
		another = rotate(another, 45.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.35,0.35));

		//Legs
		another = translate(another, vec3(0.0,0.15,0.35));
		another = rotate(another, 165.0f, vec3(1,0,0));
		another = rotate(another, 55*sin(t), vec3(0,0,1));
		this->leg->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,0.75,0.0));
		this->foot->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-0.75,0.0));
		another = rotate(another, -55*sin(t), vec3(0,0,1));
		another = rotate(another, -165.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.15,-0.35));

		another = translate(another, vec3(0.0,0.15,-0.35));
		another = rotate(another, -165.0f, vec3(1,0,0));
		another = rotate(another, -55*sin(t), vec3(0,0,1));
		this->leg->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,0.75,0.0));
		this->foot->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-0.75,0.0));
		another = rotate(another, 55*sin(t), vec3(0,0,1));
		another = rotate(another, 165.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.15,0.35));

		another = rotate(another, -90.0f, vec3(0,0,1));
		another = translate(another, vec3(-2.75,0.0,0));
		scaler = scale(another, vec3(1.0, 0.5, 0.5));
		this->head->Draw(projection, scaler, size, rotY);

		//Mouth
		another = translate(another, vec3(0.95,-0.85,0.0));
		this->mouth->Draw(projection, another, size, rotY);
		another = translate(another, vec3(-0.95,0.85,0.0));

		//Eye
		another = translate(another, vec3(-0.75,-0.85,0.5));
		scaler = scale(another, vec3(1.0, 0.35, 0.35));
		this->eye->Draw(projection, scaler, size, rotY);

		another = translate(another, vec3(-0.4,0.0,-0.3));
		another = rotate(another, 135.0f, vec3(0,1,0));
		scaler = scale(another, vec3(0.25, 0.25, 1.0));
		this->eyebrow->Draw(projection, scaler, size, rotY);
		another = rotate(another, -135.0f, vec3(0,1,0));
		another = translate(another, vec3(0.4,0.0,0.3));

		//Antennas
		another = translate(another, vec3(-0.75, 0.85, 0.15));
		another = rotate(another, 90.0f, vec3(1,0,0));
		another = rotate(another, 65.0f, vec3(0,0,1));
		this->antenna->Draw(projection, another, size, rotY);
		another = rotate(another, -65.0f, vec3(0,0,1));
		another = rotate(another, -90.0f, vec3(1,0,0));
		another = translate(another, vec3(0.0, 0.0, -1.3));
		another = rotate(another, 90.0f, vec3(1,0,0));
		another = rotate(another, 115.0f, vec3(0,0,1));
		this->antenna->Draw(projection, another, size, rotY);
		another = rotate(another, -115.0f, vec3(0,0,1));
		another = rotate(another, -90.0f, vec3(1,0,0));
		another = translate(another, vec3(0.75, -0.85, 1.15));

		//Eye
		another = translate(another, vec3(0.0,0.0,-1.0));
		scaler = scale(another, vec3(1.0, 0.35, 0.35));
		this->eye->Draw(projection, scaler, size, rotY);

		another = translate(another, vec3(-0.525,0.0,0.3));
		another = rotate(another, 45.0f, vec3(0,1,0));
		scaler = scale(another, vec3(0.25, 0.25, 1.0));
		this->eyebrow->Draw(projection, scaler, size, rotY);
		another = rotate(another, -45.0f, vec3(0,1,0));
		another = translate(another, vec3(0.525,0.0,-0.3));

		another = translate(another, vec3(1.25,-0.75,0));
	}
	//Make it dance
	else if(rotY == 1){
		another = translate(modelview, vec3(0.35,0.0,0));
		this->center->Draw(projection, another, size, rotY);
		another = translate(another, vec3(-0.35,0.0,0));

		another = translate(another, vec3(0.85,0.95,0));
		another = rotate(another, 90.0f, vec3(0,0,1));
		this->body->Draw(projection, another, size, rotY);
		this->bodyBot->Draw(projection, another, size, 0);

		//Arms
		another = translate(another, vec3(0.0,0.35,0.35));
		another = rotate(another, 45.0f, vec3(1,0,0));
		another = rotate(another, -abs(15*sin(t)), vec3(1,0,0));
		this->arm->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,1.75,0.0));
		this->hand->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-1.75,0.0));
		another = rotate(another, abs(15*sin(t)), vec3(1,0,0));
		another = rotate(another, -45.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.35,-0.35));

		another = translate(another, vec3(0.0,0.35,-0.35));
		another = rotate(another, -45.0f, vec3(1,0,0));
		another = rotate(another, -abs(15*sin(t)), vec3(1,0,0));
		this->arm->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,1.75,0.0));
		this->hand->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-1.75,0.0));
		another = rotate(another, abs(15*sin(t)), vec3(1,0,0));
		another = rotate(another, 45.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.35,0.35));

		//Legs
		another = translate(another, vec3(0.0,0.15,0.35));
		another = rotate(another, 165.0f, vec3(1,0,0));
		another = rotate(another, abs(15*sin(t)), vec3(1,0,0));
		this->leg->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,0.75,0.0));
		this->foot->Draw(projection, another, size, rotY);
		another = rotate(another, -abs(15*sin(t)), vec3(1,0,0));
		another = translate(another, vec3(0,-0.75,0.0));
		another = rotate(another, -165.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.15,-0.35));

		another = translate(another, vec3(0.0,0.15,-0.35));
		another = rotate(another, -165.0f, vec3(1,0,0));
		this->leg->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0.0,0.75,0.0));
		this->foot->Draw(projection, another, size, rotY);
		another = translate(another, vec3(0,-0.75,0.0));
		another = rotate(another, 165.0f, vec3(1,0,0));
		another = translate(another, vec3(0,-0.15,0.35));

		another = rotate(another, -90.0f, vec3(0,0,1));
		another = translate(another, vec3(-2.75,0.0,0));
		scaler = scale(another, vec3(1.0, 0.5, 0.5));
		this->head->Draw(projection, scaler, size, rotY);

		//Mouth
		another = translate(another, vec3(0.95,-0.85,0.0));
		this->mouth->Draw(projection, another, size, rotY);
		another = translate(another, vec3(-0.95,0.85,0.0));

		//Eye
		another = translate(another, vec3(-0.75,-0.85,0.5));
		scaler = scale(another, vec3(1.0, 0.35, 0.35));
		this->eye->Draw(projection, scaler, size, rotY);

		//Antennas
		another = translate(another, vec3(-0.75, 0.85, 0.15));
		another = rotate(another, 90.0f, vec3(1,0,0));
		another = rotate(another, 65.0f, vec3(0,0,1));
		this->antenna->Draw(projection, another, size, rotY);
		another = rotate(another, -65.0f, vec3(0,0,1));
		another = rotate(another, -90.0f, vec3(1,0,0));
		another = translate(another, vec3(0.0, 0.0, -1.3));
		another = rotate(another, 90.0f, vec3(1,0,0));
		another = rotate(another, 115.0f, vec3(0,0,1));
		this->antenna->Draw(projection, another, size, rotY);
		another = rotate(another, -115.0f, vec3(0,0,1));
		another = rotate(another, -90.0f, vec3(1,0,0));
		another = translate(another, vec3(0.75, -0.85, 1.15));

		//Eye
		another = translate(another, vec3(0.0,0.0,-1.0));
		scaler = scale(another, vec3(1.0, 0.35, 0.35));
		this->eye->Draw(projection, scaler, size, rotY);

		another = translate(another, vec3(1.25,-0.75,0));
	}

	if (this->GLReturnedError("Alien::Draw - on exit"))
		return;
}
