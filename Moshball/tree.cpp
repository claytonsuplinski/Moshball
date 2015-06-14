/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Create a table object out of cylinders and squares.
*/

#include <iostream>
#include "tree.h"

using namespace std;
using namespace glm;

//Creates a table object
Tree::Tree() : Object()
{
}

Tree::~Tree()
{
	this->TakeDown();
}

//Initializes all the components of the table
bool Tree::Initialize(int weather)
{
	if (this->GLReturnedError("Tree::Initialize - on entry"))
		return false;

	this->branch1 = new Cylinder2();
	this->branch2 = new Cylinder2();
	this->branch3 = new Cylinder2();
	this->branch4 = new Cylinder2();
	this->branch5 = new Cylinder2();
	this->leaves2 = new Sphere2();
	this->leaves3 = new Sphere2();
	this->leaves4 = new Sphere2();
	this->leaves5 = new Sphere2();

	vec3 branch = vec3(0.35f, 0.15f, 0.05f);
	vec3 leaf2 = vec3(0.05f, 0.35f, 0.05f);
	vec3 leaf3 = vec3(0.15f, 0.45f, 0.05f);
	
	this->branch1->texID = 6;
	this->branch1->Initialize(3,30.0f,2.0f,1.0f,"./textures/treeBark.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->branch2->texID = 6;
	this->branch2->Initialize(2,12.0f,0.8f,0.55f,"./textures/treeBark.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->branch3->texID = 6;
	this->branch3->Initialize(2,9.0f,0.2f,0.2f,"./textures/treeBark.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->branch4->texID = 6;
	this->branch4->Initialize(1,7.0f,0.2f,0.2f,"./textures/treeBark.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	this->branch5->texID = 6;
	this->branch5->Initialize(1,5.0f,0.2f,0.2f,"./textures/treeBark.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	if(weather == 0){
		this->leaves2->texID = 5;this->leaves3->texID = 5;
		this->leaves4->texID = 5;this->leaves5->texID = 5;
		this->leaves2->Initialize(2, 5, "./textures/leaves.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->leaves3->Initialize(2, 4, "./textures/leaves.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->leaves4->Initialize(2, 3, "./textures/leaves.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->leaves5->Initialize(2, 2, "./textures/leaves.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}
	else{
		this->leaves2->texID = 502;this->leaves3->texID = 502;
		this->leaves4->texID = 502;this->leaves5->texID = 502;
		this->leaves2->Initialize(2, 5, "./textures/leavesWinter.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->leaves3->Initialize(2, 4, "./textures/leavesWinter.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->leaves4->Initialize(2, 3, "./textures/leavesWinter.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
		this->leaves5->Initialize(2, 2, "./textures/leavesWinter.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag");
	}
	float x, y, z;
	branches2 = rand() % 3 + 3;
	branches3 = rand() % 4 + 3;
	branches4 = rand() % 5 + 3;
	branches5 = rand() % 6 + 3;

	branches2X = new int[branches2];
	branches2Y = new int[branches2];
	branches2Z = new int[branches2];
	for(int i=0; i<branches2; i++){
		x = (float)(rand() % 10 + 3);
		y = (float)(rand() % 360 + 1);
		z = (float)(rand() % 180 - 90);
		branches2X[i] = (int)x;
		branches2Y[i] = (int)y;
		branches2Z[i] = (int)z;
	}
	branches3X = new int[branches3];
	branches3Y = new int[branches3];
	branches3Z = new int[branches2];
	for(int i=0; i<branches3; i++){
		x = rand() % 10 + 8;
		y = rand() % 360 + 1;
		z = rand() % 180 - 90;
		branches3X[i] = x;
		branches3Y[i] = y;
		branches3Z[i] = z;
	}
	branches4X = new int[branches4];
	branches4Y = new int[branches4];
	branches4Z = new int[branches2];
	for(int i=0; i<branches4; i++){
		x = rand() % 13 + 13;
		y = rand() % 360 + 1;
		z = rand() % 180 - 90;
		branches4X[i] = x;
		branches4Y[i] = y;
		branches4Z[i] = z;
	}
	branches5X = new int[branches5];
	branches5Y = new int[branches5];
	branches5Z = new int[branches2];
	for(int i=0; i<branches5; i++){
		x = rand() % 14 + 15;
		y = rand() % 360 + 1;
		z = rand() % 180 - 90;
		branches5X[i] = x;
		branches5Y[i] = y;
		branches5Z[i] = z;
	}


	if (this->GLReturnedError("Table::Initialize - on exit"))
		return false;

	return true;
}

void Tree::StepShader(){
}

void Tree::TakeDown()
{
	this->branch1 = NULL;
	this->branch2 = NULL;
	this->branch3 = NULL;
	this->branch4 = NULL;
	this->branch5 = NULL;
	this->leaves2 = NULL;
	this->leaves3 = NULL;
	this->leaves4 = NULL;
	this->leaves5 = NULL;
	delete branches2X, branches2Y, branches3Y, branches3Y;
	delete branches2Z, branches3Z, branches4Z, branches5Z;
	delete branches4X, branches4Y, branches5Y, branches5Y;
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Position the elements of the table
void Tree::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Tree::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 another;
	another = translate(modelview, vec3(5.5,0,5.5));
	this->branch1->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0,30,0.0));
	this->leaves2->Draw(projection, another, size, 0);
	another = translate(another, vec3(0.0,-30,0.0));

	for(int i=0; i<branches2; i++){
		another = translate(another, vec3(0.0,branches2X[i],0.0));
		another = rotate(another, (float) branches2Y[i], vec3(0,1,0));
		another = rotate(another, (float) -branches2Z[i], vec3(1,0,0));
		
		this->branch2->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,12.0f,0.0));
		this->leaves2->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-12.0f,0.0));
		
		another = rotate(another, (float) branches2Z[i], vec3(1,0,0));
		another = rotate(another, (float) -branches2Y[i], vec3(0,1,0));
		another = translate(another, vec3(0.0,-branches2X[i],0.0));
	}

	for(int i=0; i<branches3; i++){
		another = translate(another, vec3(0.0,branches3X[i],0.0));
		another = rotate(another, (float) branches3Y[i], vec3(0,1,0));
		another = rotate(another, (float) -branches3Z[i], vec3(1,0,0));
		
		this->branch3->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,9.0f,0.0));
		this->leaves3->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-9.0f,0.0));
		
		another = rotate(another, (float) branches3Z[i], vec3(1,0,0));
		another = rotate(another, (float) -branches3Y[i], vec3(0,1,0));
		another = translate(another, vec3(0.0,-branches3X[i],0.0));
	}

	for(int i=0; i<branches4; i++){
		another = translate(another, vec3(0.0,branches4X[i],0.0));
		another = rotate(another, (float) branches4Y[i], vec3(0,1,0));
		another = rotate(another, (float) -branches4Z[i], vec3(1,0,0));
		
		this->branch4->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,7.0f,0.0));
		this->leaves4->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-7.0f,0.0));
		
		another = rotate(another, (float) branches4Z[i], vec3(1,0,0));
		another = rotate(another, (float) -branches4Y[i], vec3(0,1,0));
		another = translate(another, vec3(0.0,-branches4X[i],0.0));
	}

	for(int i=0; i<branches5; i++){
		another = translate(another, vec3(0.0,branches5X[i],0.0));
		another = rotate(another, (float) branches5Y[i], vec3(0,1,0));
		another = rotate(another, (float) -branches5Z[i], vec3(1,0,0));
		
		this->branch5->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,6.0f,0.0));
		this->leaves5->Draw(projection, another, size, 0);
		another = translate(another, vec3(0.0,-6.0f,0.0));
		
		another = rotate(another, (float) branches5Z[i], vec3(1,0,0));
		another = rotate(another, (float) -branches5Y[i], vec3(0,1,0));
		another = translate(another, vec3(0.0,-branches5X[i],0.0));
	}


	if (this->GLReturnedError("Table::Draw - on exit"))
		return;
}
