/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Creates a tiger object out of cylinders, discs, and spheres.
*/

#pragma once
#include "object.h"
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader_utils.h"

class Text : public Object
{
public:
	Text();
	~Text();
	virtual bool Initialize(); // int init_resources();
	void TakeDown();  // void free_resources();
	void StepShader();
	Shader shader;
	Shader solid_color;

	////////Font variables//////////
GLuint program;GLint attribute_coord;GLint uniform_tex;GLint uniform_color;
struct point {GLfloat x;GLfloat y;GLfloat s;GLfloat t;};
GLuint vbo;FT_Library ft;FT_Face face;const char *fontfilename;
	
	void render_text(const char *text, float x, float y, float sx, float sy);

	void activeTextDisplay(const char* words, float x, float y);
	
	
	int main(int argc, char *argv[]);

private:
	glm::vec4 colors[2];
	std::vector<VertexAttributesPCN> vertices;
	typedef Object super;
};