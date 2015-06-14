/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Creates a textured bowl shape
*/

#include <iostream>
#include "bowl2.h"

using namespace std;
using namespace glm;

Bowl2::Bowl2() : Object2(){this->shader_index = 0;}

void Bowl2::StepShader(){this->shader_index = ++this->shader_index % this->shaders.size();}

bool Bowl2::Initialize(int slices, float radius, char* filename, char* v, char* f){
	if (this->GLReturnedError("Bowl2::Initialize - on entry"))
		return false;

	if(slices<1){slices = 1;}

#pragma region Shader code should not be here for better O.O.

	if (!this->basic_texture_shader.Initialize(v,f))
		return false;

	this->shaders.push_back(&this->basic_texture_shader);
#pragma endregion

#pragma region Code to make FreeImage work

	if (!TextureManager::Inst()->LoadTexture((const char *) filename, texID))
		return false;

	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
#pragma endregion

	slices *= 4;
	mat4 m;

	const vec3 n = normalize(vec3(0.5f, 1.0f, 0.0f));
	const vec4 x_axis(radius, 0.0f, 0.0f, 1.0f);
	const vec3 y_axis(0.0f, 1.0f, 0.0f);
	const vec3 z_axis(0.0f, 0.0f, 1.0f);
	const vec3 center(0.0f, 0.0f, 0.0f);
	const float increment =  360.0f / float(slices); 
	int incrCount = 0;

	for (int i = 0; i < slices/2; ++i){
		for (int j = 0; j < slices; ++j){
			VertexAttributes cur_vertex , nxt_vertex, dwn_vertex, dwr_vertex;

			cur_vertex.color = vec3(0,0,0);dwn_vertex.color = vec3(1,0,0);
			dwr_vertex.color = vec3(1,1,0);nxt_vertex.color = vec3(0,1,0);
			
			cur_vertex.position = vec3(m * x_axis);
			cur_vertex.normal = normalize(cur_vertex.position);
			cur_vertex.texture_coordinate = vec2(1.0f, 1.0f);

			m = rotate(m, -increment, z_axis);

			dwn_vertex.position = vec3(m * x_axis);
			dwn_vertex.normal = normalize(dwn_vertex.position);
			dwn_vertex.texture_coordinate = vec2(0.0f, 1.0f);

			m = rotate(m, increment, z_axis);
			m = rotate(m, -incrCount*increment, z_axis);
			m = rotate(m, increment, y_axis);
			m = rotate(m, incrCount*increment, z_axis);
			m = rotate(m, -increment, z_axis);

			dwr_vertex.position = vec3(m * x_axis);
			dwr_vertex.normal = normalize(dwr_vertex.position);
			dwr_vertex.texture_coordinate = vec2(0.0f, 0.0f);

			m = rotate(m, increment, z_axis);
			m = rotate(m, -incrCount*increment, z_axis);
			m = rotate(m, -increment, y_axis);			
			m = rotate(m, increment, y_axis);
			m = rotate(m, incrCount*increment, z_axis);
		
			nxt_vertex.position = vec3(m * x_axis);
			nxt_vertex.normal = normalize(nxt_vertex.position);
			nxt_vertex.texture_coordinate = vec2(1.0f, 0.0f);
		
			this->vertices.push_back(dwr_vertex);this->vertices.push_back(dwn_vertex);
			this->vertices.push_back(nxt_vertex);this->vertices.push_back(cur_vertex);
	
			this->vertex_indices.push_back(this->vertices.size() - 1);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 3);

			this->vertex_indices.push_back(this->vertices.size() - 3);
			this->vertex_indices.push_back(this->vertices.size() - 2);
			this->vertex_indices.push_back(this->vertices.size() - 4);
		}
		m = rotate(m, increment, z_axis);
		incrCount++;
	}

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3) * 3));

	//	Each of the attributes to be used must be enabled.
	glEnableVertexAttribArray(0);glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Bowl2::Initialize - on exit"))
		return false;

	return true;
}

void Bowl2::TakeDown(){super::TakeDown();}

void Bowl2::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time){
	if (this->GLReturnedError("Bowl2::Draw - on entry"))
		return;

	mat4 mvp = projection * modelview;

	this->shaders[this->shader_index]->Use();
	//Bind the texture with the unique texture id
	TextureManager::Inst()->BindTexture(texID, 0);
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(transpose(inverse(modelview))));
	this->shaders[this->shader_index]->CustomSetup();

	glBindVertexArray(this->vertex_array_handle);
#ifdef DRAW_POINTS
	glDrawArrays(GL_POINTS, 0, this->vertex_indices.size());
#else
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
#endif

	glUseProgram(0);glBindVertexArray(0);

	if (this->GLReturnedError("Bowl2::Draw - on exit"))
		return;
}
