/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a local textured shader object
-Taken from demo code
*/

#pragma once
#include "shader2.h"

class TexturedShader : public Shader2
{
public:
	TexturedShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup();

private:
	typedef Shader2 super;
	GLuint texture_handle;
};
