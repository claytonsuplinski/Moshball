/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a local textured shader object
-Taken from demo code
*/

/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department
*/

#include "texturedshader.h"

/*	BAD DESIGN WARNING...
	This code has not been properly planned and was tossed in right before lecture.
	The useful concepts are that a texture has to be bound to a texture unit (which
	has been hard coded to zero as the shaders making use of this texture will expect
	the sampler2D to be on zero.
*/

TexturedShader::TexturedShader() : super()
{
	this->texture_handle = GLuint(-1);
}

bool TexturedShader::Initialize(char * vertex_shader_file, char * fragment_shader_file)
{
	if (super::Initialize(vertex_shader_file, fragment_shader_file) == false)
		return false;

	this->Use();
	this->texture_handle = glGetUniformLocation(this->program_id, (const GLchar *) "picture");
	glUseProgram(0);
	return true;
}

void TexturedShader::CustomSetup()
{
	glUniform1i(this->texture_handle, 0);
}
