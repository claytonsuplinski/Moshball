/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Create a cylinder object.
*/

#include <iostream>
#include "precip.h"

using namespace std;
using namespace glm;

//Creates a cylinder object 
Precip::Precip() : Object()
{
	this->shader_index = 0;
}


//Initializes a cylinder with the given dimensions.
bool Precip::Initialize(char* v, char* f)
{
	if (this->GLReturnedError("Precip::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	VertexAttributesPCN cur_vertex , nxt_vertex, dwn_vertex, dwr_vertex;
	VertexAttributesP cur_vertexN , nxt_vertexN, dwn_vertexN, dwr_vertexN;

	cur_vertex.position = vec3(0.0f, 0.0f, 0.0f);
	cur_vertex.color = color;
	cur_vertex.normal = normalize(vec3(1.0f, 1.0f, 1.0f));

	this->vertices.push_back(cur_vertex);
	this->normal_vertices.push_back(cur_vertexN);
	
	this->vertex_indices.push_back(this->vertices.size() - 1);
	this->normal_indices.push_back(this->vertices.size() - 1);

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	//Create the triangles from the vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Place the normals right after the vertices in the buffer
	if (this->normal_vertices.size() > 0)
	{
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	if (!this->phong_shader.Initialize(v,f))
		return false;

	this->shaders.push_back(&this->phong_shader);

	if (this->GLReturnedError("Precip::Initialize - on exit"))
		return false;

	return true;
}

//Deletes the cylinder object
void Precip::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw the cylinder object
void Precip::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float radius)
{
	if (this->GLReturnedError("Precip::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	//Activate the current shader
	this->shaders[this->shader_index]->Use();
	this->shaders[this->shader_index]->CommonSetup(0, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	glBindVertexArray(this->vertex_array_handle);
	glPointSize(5.0f);
	glDrawElements(GL_POINTS , 1, GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	glUseProgram(0);

	if (this->GLReturnedError("Cylinder::Draw - on exit"))
		return;
}
