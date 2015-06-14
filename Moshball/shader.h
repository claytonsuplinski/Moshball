/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a general shader object
-Taken from demo code
*/

/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department
*/

#pragma once
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	void TakeDown();
	void Use();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup();
	void CommonSetup(const float time, const GLint * size, const GLfloat * projection, const GLfloat * modelview, const GLfloat * mvp, const GLfloat * nm);

	GLuint modelview_matrix_handle;
	GLuint projection_matrix_handle;
	GLuint normal_matrix_handle;
	GLuint mvp_handle;
	GLuint size_handle;
	GLuint time_handle;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;
	bool LoadShader(const char * file_name, GLuint shader_id);
	std::stringstream GetShaderLog(GLuint shader_id);

protected:
	bool GLReturnedError(char * s);
};

class BackgroundShader : public Shader
{
public:
	BackgroundShader();
	virtual bool Initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void CustomSetup(glm::vec4 * color_array);

protected:
	GLuint color_array_handle;

private:
	typedef Shader super;
};
