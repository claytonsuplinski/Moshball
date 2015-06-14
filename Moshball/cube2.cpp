/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a textured cube with 6 different images
(used only as skybox)
*/

#include <iostream>
#include "cube2.h"

using namespace std;
using namespace glm;

//Create the cube object
Cube2::Cube2() : Object()
{
}

Cube2::~Cube2(){
	this->TakeDown();
}

//Initialize all the elements of the cube object
bool Cube2::Initialize(int skyboxID, int baseTexID, char* v, char* f)
{
	if (this->GLReturnedError("Cube2::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->left = new Square2();this->right = new Square2();
	this->up = new Square2();this->down = new Square2();
	this->front = new Square2();this->back = new Square2();

	char *leftBox, *rightBox, *topBox, *botBox, *frontBox, *backBox;

	switch (skyboxID){
	case 0: //Normal Weather
		leftBox="./skyboxMain/frontsh.jpg";rightBox="./skyboxMain/backsh.jpg";
		topBox="./skyboxMain/toptsh.jpg";botBox="./skyboxMain/botsh.jpg";
		frontBox="./skyboxMain/leftsh.jpg";backBox="./skyboxMain/rightsh.jpg";
		break;
	case 1: //Winter
		leftBox="./skyboxWinter2/frontgaz.jpg";rightBox="./skyboxWinter2/backgaz.jpg";
		topBox="./skyboxWinter2/topgaz.jpg";botBox="./skyboxWinter2/botgaz.jpg";
		frontBox="./skyboxWinter2/leftgaz.jpg";backBox="./skyboxWinter2/rightgaz.jpg";
		break;
	case 2: //Rain
		leftBox="./skyboxWinter/frontdark.jpg";rightBox="./skyboxWinter/backdark.jpg";
		topBox="./skyboxWinter/topdark.jpg";botBox="./skyboxWinter/botdark.jpg";
		frontBox="./skyboxWinter/leftdark.jpg";backBox="./skyboxWinter/rightdark.jpg";
		break;
	case 3: //Underwater
		leftBox="./skyboxUnderwater/aquabk.jpg";rightBox="./skyboxUnderwater/aquaft.jpg";
		topBox="./skyboxUnderwater/aquaup.jpg";botBox="./skyboxUnderwater/aquadn.jpg";
		frontBox="./skyboxUnderwater/aqualf.jpg";backBox="./skyboxUnderwater/aquart.jpg";
		break;
	default:
		leftBox="./skyboxMain/frontsh.jpg";rightBox="./skyboxMain/backsh.jpg";
		topBox="./skyboxMain/toptsh.jpg";botBox="./skyboxMain/botsh.jpg";
		frontBox="./skyboxMain/leftsh.jpg";backBox="./skyboxMain/rightsh.jpg";
		break;
	}

	this->left->texID = baseTexID;this->right->texID = baseTexID+1;this->up->texID = baseTexID+2;
	this->down->texID = baseTexID+3;this->front->texID = baseTexID+4;this->back->texID = baseTexID+5;
	this->left->Initialize(300.0f,1,leftBox,v,f);this->right->Initialize(300.0f,1,rightBox,v,f);	
	this->up->Initialize(300.0f,1,topBox,v,f);this->down->Initialize(300.0f,1,botBox,v,f);	
	this->front->Initialize(300.0f,1,frontBox,v,f);this->back->Initialize(300.0f,1,backBox,v,f);	
	
	if (this->GLReturnedError("Cube2::Initialize - on exit"))
		return false;

	return true;
}

void Cube2::StepShader(){
	this->left->StepShader();this->right->StepShader();
	this->up->StepShader();this->down->StepShader();
	this->front->StepShader();this->back->StepShader();
}

//Delete the cube object
void Cube2::TakeDown()
{
	this->left = NULL;this->right = NULL;
	this->up = NULL;this->down = NULL;
	this->front = NULL;this->back = NULL;
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw and position the elements of the cube
void Cube2::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Cube2::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 another, scaler;

	another = modelview;
	another = translate(another, vec3(150.0f, 0.0f, 0.0f));
	another = rotate(another, 90.0f, vec3(0,1,0));
	this->left->Draw(projection, another, size, 0);
	another = rotate(another, -90.0f, vec3(0,1,0));
	another = translate(another, vec3(-300.0f, 0.0f, 0.0f));
	another = rotate(another, -90.0f, vec3(0,1,0));
	this->right->Draw(projection, another, size, 0);

	another = modelview;
	another = translate(another, vec3(0.0f,150.0f,0.0f));
	another = rotate(another, 90.0f, vec3(0,1,0));
	another = rotate(another, -90.0f, vec3(1,0,0));
	this->up->Draw(projection, another, size, 0);

	another = modelview;
	another = translate(another, vec3(0.0f,-150.0f,0.0f));
	another = rotate(another, 90.0f, vec3(0,1,0));
	another = rotate(another, 90.0f, vec3(1,0,0));
	this->down->Draw(projection, another, size, 0);

	another = modelview;
	another = translate(another, vec3(0.0f,0.0f,150.0f));	
	this->front->Draw(projection, another, size, 0);

	another = modelview;
	another = translate(another, vec3(0.0f,0.0f,-150.0f));
	another = rotate(another, 180.0f, vec3(0,1,0));
	this->back->Draw(projection, another, size, 0);

	if (this->GLReturnedError("Cube2::Draw - on exit"))
		return;
}
