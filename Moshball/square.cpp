/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a square object
*/

#include <iostream>
#include "square.h"

using namespace std;
using namespace glm;

//Creates the square object
Square::Square() : Object(){
	this->shader_index = 0;
	this->grassShader = false;
	this->grassFirst = 1;
}

//Create a square with the specified parameters
//x = # of subsquares; s = size of total square
bool Square::Initialize(int x, float s, char* v, char* f)
{
	if (this->GLReturnedError("Square::Initialize - on entry"))
		return false;

	if (!super::Initialize())
		return false;

	if (x < 1)
		x = 2;

	mat4 m;

	const vec3 n = normalize(vec3(0.5f, 1.0f, 0.0f));
	const vec4 x_axis(1.0f, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const vec3 z_axis(0.0f, 0.0f, 1.0f);
	const vec3 center(0.0f, 0.0f, 0.0f);
	const float increment =  s / float(x); 
	int incrCount = 0;
	int checker = 0;

	for (int i = 0; i < x; ++i){
		for (int j = 0; j < x; ++j){
			VertexAttributesPCN cur_vertex , nxt_vertex, dwn_vertex, dwr_vertex;
			VertexAttributesP cur_vertexN , nxt_vertexN, dwn_vertexN, dwr_vertexN;

			if(grassShader){
				cur_vertex.color = vec3(0,0,0);dwn_vertex.color = vec3(1,0,0);
				dwr_vertex.color = vec3(1,1,0);nxt_vertex.color = vec3(0,1,0);
			}
			else if(checker%2 == 0){
				cur_vertex.color = color1;dwn_vertex.color = color1;
				dwr_vertex.color = color1;nxt_vertex.color = color1;
			}
			else{
				cur_vertex.color = color2;dwn_vertex.color = color2;
				dwr_vertex.color = color2;nxt_vertex.color = color2;
			}
			
			cur_vertex.position = vec3(m * x_axis);
			cur_vertex.normal = vec3(0,1,0);

			m = translate(m, vec3(0.0f,0.0f,-increment));
			dwn_vertex.position = vec3(m * x_axis);			
			dwn_vertex.normal = vec3(0,1,0);

			m = translate(m, vec3(-increment, 0.0f,0.0f));
			dwr_vertex.position = vec3(m * x_axis);			
			dwr_vertex.normal = vec3(0,1,0);
			
			m = translate(m, vec3(0.0f,0.0f,increment));
			nxt_vertex.position = vec3(m * x_axis);			
			nxt_vertex.normal = vec3(0,1,0);

			cur_vertexN.position = cur_vertex.normal;
			dwr_vertexN.position = dwr_vertex.normal;
			nxt_vertexN.position = nxt_vertex.normal;
			dwn_vertexN.position = dwn_vertex.normal;
		
			this->vertices.push_back(dwr_vertex);this->vertices.push_back(dwn_vertex);
			this->vertices.push_back(nxt_vertex);this->vertices.push_back(cur_vertex);

			this->normal_vertices.push_back(dwr_vertexN);
			this->normal_vertices.push_back(dwn_vertexN);
			this->normal_vertices.push_back(nxt_vertexN);
			this->normal_vertices.push_back(cur_vertexN);
	
			this->vertex_indices.push_back(this->vertices.size() - 4);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 3);

			this->vertex_indices.push_back(this->vertices.size() - 3);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 1);

			this->normal_indices.push_back(this->vertices.size() - 4);
			this->normal_indices.push_back(this->vertices.size() - 2);
			this->normal_indices.push_back(this->vertices.size() - 3);

			this->normal_indices.push_back(this->vertices.size() - 3);
			this->normal_indices.push_back(this->vertices.size() - 2);
			this->normal_indices.push_back(this->vertices.size() - 1);

			cur_vertex.normal = -cur_vertex.normal;dwn_vertex.normal = -dwn_vertex.normal;
			dwr_vertex.normal = -dwr_vertex.normal;nxt_vertex.normal = -nxt_vertex.normal;

			cur_vertexN.position = cur_vertex.normal;dwr_vertexN.position = dwr_vertex.normal;
			nxt_vertexN.position = nxt_vertex.normal;dwn_vertexN.position = dwn_vertex.normal;

			this->vertices.push_back(dwr_vertex);this->vertices.push_back(dwn_vertex);
			this->vertices.push_back(nxt_vertex);this->vertices.push_back(cur_vertex);

			this->normal_vertices.push_back(dwr_vertexN);this->normal_vertices.push_back(dwn_vertexN);
			this->normal_vertices.push_back(nxt_vertexN);this->normal_vertices.push_back(cur_vertexN);

			this->vertex_indices.push_back(this->vertices.size() - 1);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 3);

			this->vertex_indices.push_back(this->vertices.size() - 3);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 4);

			this->normal_indices.push_back(this->vertices.size() - 4);
			this->normal_indices.push_back(this->vertices.size() - 2);
			this->normal_indices.push_back(this->vertices.size() - 3);

			this->normal_indices.push_back(this->vertices.size() - 3);
			this->normal_indices.push_back(this->vertices.size() - 2);
			this->normal_indices.push_back(this->vertices.size() - 1);
			checker++;
		}
		m = translate(m, vec3((x*increment), 0.0f,0.0f));
		m = translate(m, vec3(0.0f,0.0f,-increment));
		incrCount++;
		if(x%2 == 0){checker++;}
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributesPCN), &this->vertices[0]))
		return false;

	if(!grassShader){
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
		glEnableVertexAttribArray(0);glEnableVertexAttribArray(1);glEnableVertexAttribArray(2);
	}
	else{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));
		glEnableVertexAttribArray(0);glEnableVertexAttribArray(1);
		
	}
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

	if (this->GLReturnedError("Square::Initialize - on exit"))
		return false;

	return true;
}

//Delete the square object
void Square::TakeDown()
{
	this->vertices.clear();this->shader.TakeDown();this->solid_color.TakeDown();
	super::TakeDown();
}

//Draw the square object
void Square::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Square::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));
	if(grassShader){
		if(grassFirst == 1){origMVP = mvp;origNM = nm;grassFirst = 2;}
		mat4 mvp = origMVP;
		mat3 nm = origNM;
	}
	

	this->shaders[this->shader_index]->Use();
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);glUseProgram(0);

	if (this->GLReturnedError("Square::Draw - on exit"))
		return;
}