/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates the square arena
*/

#include <iostream>
#include "squareStadium.h"

using namespace std;
using namespace glm;

//Create the SquareStadium object
SquareStadium::SquareStadium() : Object()
{
}

SquareStadium::~SquareStadium(){
	this->TakeDown();
}

//Initialize all the elements of the SquareStadium
bool SquareStadium::Initialize(int weatherIndex)
{
	if (this->GLReturnedError("SquareStadium::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	this->wall = new Square3();this->floor = new Square2();this->bleacher = new Bleacher();
	this->grass = new Cylinder();this->bird = new Bird();this->bird2 = new Bird();
	this->bird3 = new Bird();this->bird4 = new Bird();this->jb = new Jumbotron();
	this->zubat = new Gengar();this->charizard = new Gengar();this->blades = new Gengar();
	this->mtns = new Gengar();this->trees = new Gengar();

	this->jb->Initialize();
	
	if(weatherIndex == 0){
		this->floor->texID = 28;
		this->floor->Initialize(1000.0f, 200, "./textures/grass.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}
	else if(weatherIndex == 1){
		this->floor->texID = 500;
		this->floor->Initialize(1000.0f, 200, "./textures/grassRain.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}
	else{
		this->floor->texID = 501;
		this->floor->Initialize(1000.0f, 200, "./textures/grassWinter.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}

	this->wall->texID = 10;
	this->wall->Initialize(12,105.6f, "./textures/clothTexture.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->grass->color = vec3(0.05f, 0.65f, 0.2f);
	this->grass->Initialize(1,0.75f, 0.03f, 0.03f, "phong.vert", "phong.frag");

	this->zubat->texID = 23;
	this->zubat->Initialize("./models/zubat.obj", "./models/zubat.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	this->charizard->texID = 24;
	this->charizard->Initialize("./models/charizard.obj", "./models/charizard.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");

	if(weatherIndex < 2){
		this->blades->texID = 45;
		this->blades->Initialize("./models/Grass2.obj", "./models/Grass1.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->mtns->texID = 47;
		this->mtns->Initialize("./models/mtnPass.obj", "./models/mtnPass.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->trees->texID = 49;
		this->trees->Initialize("./models/GumBough.obj", "./models/GumBough.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}
	else{
		this->blades->texID = 46;
		this->blades->Initialize("./models/Grass2.obj", "./models/Grass1Wnt.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->mtns->texID = 48;
		this->mtns->Initialize("./models/mtnPass.obj", "./models/mtnPassWnt.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->trees->texID = 490;
		this->trees->Initialize("./models/GumBough.obj", "./models/GumBoughWnt.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}

	vec3 yellow = vec3(0.8, 0.8, 0.1);vec3 red = vec3(0.8, 0.1, 0.1);vec3 black = vec3(0.0, 0.0, 0.0);
	vec3 white = vec3(1.0, 1.0, 1.0);vec3 blue = vec3(0.1, 0.1, 0.75);vec3 green = vec3(0.1, 0.45, 0.1);
	this->bird->Initialize(yellow, red, black, red);this->bird2->Initialize(red, yellow, green, blue);this->bird3->Initialize(white, blue, white, white);
	this->bird4->Initialize(yellow, black, black, black);

	this->bleacher->Initialize();

	numBirds = 12; //Give birds initial positions
	birdX = new int[numBirds];birdY = new int[numBirds];birdType = new int[numBirds];birdOffset = new int[numBirds];
	for(int i=0; i<numBirds; i++){
		birdX[i] = rand() % 106 - 106;birdY[i] = rand() % 15 + 5;
		birdType[i] = rand() % 4 + 1;birdOffset[i] = rand() % 360 + 1;
	}

	numGrass = 78;
	int curr = 0; //Position grass in middle of field (also used for mountains)
	grassX = new int[numGrass];grassY = new int[numGrass];
	for(float i=-106; i<0; i+=12){
		for(float j=-106; i<0; i+=12){
			grassX[curr] = (int)(i);grassY[curr] = (int)(j + 53);curr++;
		}
	}

	if (this->GLReturnedError("SquareStadium::Initialize - on exit"))
		return false;

	return true;
}

void SquareStadium::StepShader(){
	this->floor->StepShader();this->bleacher->StepShader();this->grass->StepShader();
	this->bird->StepShader();this->bird2->StepShader();this->bird3->StepShader();
	this->bird4->StepShader();this->jb->StepShader();
}

//Delete the SquareStadium
void SquareStadium::TakeDown()
{
	this->wall = NULL;this->floor = NULL;this->bleacher = NULL;this->grass = NULL;
	this->bird = NULL;this->bird2 = NULL;this->bird3 = NULL;this->bird4 = NULL;
	this->jb = NULL;this->zubat = NULL;this->charizard = NULL;this->blades = NULL;
	this->mtns = NULL;this->trees = NULL;this->vertices.clear();this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw and position the elements of the SquareStadium
void SquareStadium::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("SquareStadium::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 another, scaler;

	float t = float(glutGet(GLUT_ELAPSED_TIME) / 5000.0f);
	float tOffset = float(glutGet(GLUT_ELAPSED_TIME) / 1000.0f) - 1000;

	for(int i=0; i<numGrass; i+=2){ //Grass
		another = translate(modelview, vec3(grassX[i],0.0,grassY[i]));
		scaler = scale(another, vec3(12.0f, 0.75f, 12.0f)); 
		glCullFace(GL_FRONT);this->blades->Draw(projection, scaler, size, t);
		glCullFace(GL_BACK);this->blades->Draw(projection, scaler, size, t);
		another = translate(another, vec3(-grassX[i],0.0,-grassY[i]));
	}

	for(int i=0; i<numGrass; i+=2){ //Mountains
		another = translate(modelview, vec3(grassX[i],0.0,grassY[i]));
		scaler = another;
		scaler = translate(another, vec3(0.0f, 0.0f, 62.0f));
		scaler = scale(scaler, vec3(12.0f, 3.0f, 1.0f));
		this->mtns->Draw(projection, scaler, size, t);
		scaler = translate(another, vec3(0.0f, 0.0f, -72.0f));
		scaler = scale(scaler, vec3(12.0f, 3.0f, 1.0f));
		this->mtns->Draw(projection, scaler, size, t);
		another = translate(another, vec3(-grassX[i],0.0,-grassY[i]));
	}

	for(int i=0; i<numBirds; i++){ //Birds
		another = translate(modelview, vec3(birdX[i],birdY[i],500*tan(t+birdOffset[i])));
		another = rotate(another, 270.0f, vec3(1,0,0));
		another = rotate(another, 90.0f, vec3(0,1,0));
		if(birdType[i] == 1){this->bird->Draw(projection, another, size, 0);}
		else if(birdType[i] == 2){this->bird2->Draw(projection, another, size, 0);}
		else if(birdType[i] == 3){
			mat4 scaler;
			scaler = rotate(another, -90.0f, vec3(0,1,0));
			scaler = rotate(scaler, -210.0f, vec3(1,0,0));
			scaler = scale(scaler, vec3(0.1f, 0.1f, 0.1f));
			this->charizard->Draw(projection, scaler, size, 0);
		}
		else{
			mat4 scaler;
			scaler = rotate(another, -90.0f, vec3(0,1,0));
			scaler = rotate(scaler, -270.0f, vec3(1,0,0));
			scaler = scale(scaler, vec3(0.005f, 0.005f, 0.005f));
			this->zubat->Draw(projection, scaler, size, 0);
		}		
	}

	//Elements of the arena.
	another = modelview;
	another = translate(modelview, vec3(-500.0f, 0.0f, -500.0f));
	another = rotate(another, 90.0f, vec3(1.0f, 0.0f, 0.0f));	
	this->floor->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-25.0f, -0.30f, 4.0f));
	this->trees->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-55.0f, 0.0f, 3.0f));
	another = rotate(another, 180.0f, vec3(0, 1, 0));
	this->jb->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-95.0f, -0.5f, 2.0f));
	another = rotate(another, 45.0f, vec3(0, 1, 0));
	this->trees->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-15.0f, 0.0f, -111.0f));
	this->trees->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-32.0f, 0.0f, -107.0f));
	this->jb->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-55.0f, -0.5f, -107.0f));
	another = rotate(another, 75.0f, vec3(0, 1, 0));
	this->trees->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-78.0f, 0.0f, -107.0f));
	this->jb->Draw(projection, another, size, 0);

	another = translate(modelview, vec3(-105.0f, 0.0f, -118.0f));
	another = rotate(another, 145.0f, vec3(0, 1, 0));
	this->trees->Draw(projection, another, size, 0);

	scaler = rotate(modelview, 90.0f, vec3(1.0f, 0.0f, 0.0f));
	scaler = scale(scaler, vec3(1.0f, 1.0f, 0.02f));	
	this->wall->Draw(projection, scaler, size, 0);

	scaler = translate(modelview, vec3(0.0f, 0.0f, -52.8f));
	this->bleacher->Draw(projection, scaler, size, 0);
	scaler = translate(modelview, vec3(0.0f, 0.0f, 52.8f));

	scaler = translate(modelview, vec3(-103.6f, 0.0f, -52.8f));
	scaler = rotate(scaler, 180.0f, vec3(0, 1, 0));
	this->bleacher->Draw(projection, scaler, size, 0);
	scaler = rotate(scaler, -180.0f, vec3(0, 1, 0));
	scaler = translate(modelview, vec3(103.6f, 0.0f, 52.8f));

	scaler = translate(modelview, vec3(0.0f, 0.0f, -105.6f));
	scaler = rotate(scaler, 90.0f, vec3(1.0f, 0.0f, 0.0f));
	scaler = scale(scaler, vec3(1.0f, 1.0f, 0.02f));	
	this->wall->Draw(projection, scaler, size, 0);

	scaler = translate(modelview, vec3(1.0f, 0.0f, -1.0f));
	scaler = rotate(scaler, -90.0f, vec3(0.0f, 1.0f, 0.0f));
	scaler = rotate(scaler, 90.0f, vec3(1.0f, 0.0f, 0.0f));	
	scaler = scale(scaler, vec3(1.0f, 1.0f, 0.02f));	
	this->wall->Draw(projection, scaler, size, 0);

	scaler = translate(modelview, vec3(-104.6f, 0.0f, -1.0f));
	scaler = rotate(scaler, -90.0f, vec3(0.0f, 1.0f, 0.0f));
	scaler = rotate(scaler, 90.0f, vec3(1.0f, 0.0f, 0.0f));	
	scaler = scale(scaler, vec3(1.0f, 1.0f, 0.02f));	
	this->wall->Draw(projection, scaler, size, 0);	

	if (this->GLReturnedError("SquareStadium::Draw - on exit"))
		return;
}
