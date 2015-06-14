/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a triangle object
*/

#include <iostream>
#include "triangle.h"

using namespace std;
using namespace glm;

//Creates the triangle object
Triangle::Triangle() : Object()
{
	this->shader_index = 0;
}

//Create a triangle with the specified parameters
bool Triangle::Initialize(float s, char* v, char* f)
{
	if (this->GLReturnedError("Triangle::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	mat4 m;

	const vec3 n = normalize(vec3(0.5f, 1.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const vec3 z_axis(0.0f, 0.0f, 1.0f);
	const vec3 center(0.0f, 0.0f, 0.0f);
	const float increment =  s; 
	int incrCount = 0;int checker = 0;


	VertexAttributesPCN cur_vertex , nxt_vertex, dwn_vertex;
	VertexAttributesP cur_vertexN , nxt_vertexN, dwn_vertexN;

	cur_vertex.color = color;dwn_vertex.color = color;nxt_vertex.color = color;
			
	cur_vertex.position = vec3(m * x_axis);cur_vertex.normal = vec3(0,1,0);

	m = translate(m, vec3(0.0f,0.0f,-increment));
	dwn_vertex.position = vec3(m * x_axis);			
	dwn_vertex.normal = vec3(0,1,0);

	m = translate(m, vec3(-increment, 0.0f,0.0f));			
	m = translate(m, vec3(0.0f,0.0f,increment));
	nxt_vertex.position = vec3(m * x_axis);			
	nxt_vertex.normal = vec3(0,1,0);

	cur_vertexN.position = cur_vertex.normal;nxt_vertexN.position = nxt_vertex.normal;
	dwn_vertexN.position = dwn_vertex.normal;
		
	this->vertices.push_back(dwn_vertex);this->vertices.push_back(nxt_vertex);
	this->vertices.push_back(cur_vertex);

	this->normal_vertices.push_back(dwn_vertexN);this->normal_vertices.push_back(nxt_vertexN);
	this->normal_vertices.push_back(cur_vertexN);

	this->vertex_indices.push_back(this->vertices.size() - 3);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 1);

	this->normal_indices.push_back(this->vertices.size() - 3);
	this->normal_indices.push_back(this->vertices.size() - 2);
	this->normal_indices.push_back(this->vertices.size() - 1);

	cur_vertex.normal = -cur_vertex.normal;dwn_vertex.normal = -dwn_vertex.normal;
	nxt_vertex.normal = -nxt_vertex.normal;

	cur_vertexN.position = cur_vertex.normal;nxt_vertexN.position = nxt_vertex.normal;
	dwn_vertexN.position = dwn_vertex.normal;

	this->vertices.push_back(dwn_vertex);this->vertices.push_back(nxt_vertex);
	this->vertices.push_back(cur_vertex);

	this->normal_vertices.push_back(dwn_vertexN);this->normal_vertices.push_back(nxt_vertexN);
	this->normal_vertices.push_back(cur_vertexN);

	this->vertex_indices.push_back(this->vertices.size() - 1);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 3);

	this->normal_indices.push_back(this->vertices.size() - 1);
	this->normal_indices.push_back(this->vertices.size() - 2);
	this->normal_indices.push_back(this->vertices.size() - 3);


	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glEnableVertexAttribArray(0);glEnableVertexAttribArray(1);glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

	if (this->GLReturnedError("Triangle::Initialize - on exit"))
		return false;

	return true;
}

//Delete the triangle object
void Triangle::TakeDown()
{
	this->vertices.clear();
	this->shader.TakeDown();
	this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw the triangle object
void Triangle::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Triangle::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);
	glUseProgram(0);

	if (this->GLReturnedError("Triangle::Draw - on exit"))
		return;
}
