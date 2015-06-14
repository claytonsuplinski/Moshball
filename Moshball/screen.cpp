#include <iostream>
#include "screen.h"

using namespace std;
using namespace glm;

Screen::Screen() : Object2()
{
	this->shader_index = 0;
}

void Screen::StepShader()
{
	this->shader_index = ++this->shader_index % this->shaders.size();
}

bool Screen::Initialize(float x, float y)
{
	if (this->GLReturnedError("Screen::Initialize - on entry"))
		return false;

#pragma region Shader code should not be here for better O.O.
	//	Nothing shader related ought to be here for better O.O.

	if (!this->basic_texture_shader.Initialize("basic_texture_shader.vert", "basic_texture_shader.frag"))
		return false;

	this->shaders.push_back(&this->basic_texture_shader);
#pragma endregion

#pragma region Code to make FreeImage work
	//	The picture too, ought not to be here.

	//if (!TextureManager::Inst()->LoadTexture((const char *) filename, texID))
		//return false;

	//	The current "TextureManager" that comes with FreeImage is quite dumb.
	glTexEnvf(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_REPLACE);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR);
#pragma endregion

	if(x == 0){x = 1.0f;}if(y == 0){y = 1.0f;}

	VertexAttributes cur_vertex , nxt_vertex, dwn_vertex, dwr_vertex;
	cur_vertex.position = vec3(0.0f, y, 0.0f);
	cur_vertex.normal = vec3(0.0f, 0.0f, 1.0f);
	cur_vertex.texture_coordinate = vec2(0.0f, 1.0f);

	dwn_vertex.position = vec3(0.0f, 0.0f, 0.0f);			
	dwn_vertex.normal = vec3(0.0f, 0.0f, 1.0f);
	dwn_vertex.texture_coordinate = vec2(0.0f, 0.0f);

	dwr_vertex.position = vec3(x, 0.0f, 0.0f);			
	dwr_vertex.normal = vec3(0.0f, 0.0f, 1.0f);
	dwr_vertex.texture_coordinate = vec2(1.0f, 0.0f);

	nxt_vertex.position = vec3(x, y, 0.0f);			
	nxt_vertex.normal = vec3(0.0f, 0.0f, 1.0f);
	nxt_vertex.texture_coordinate = vec2(1.0f, 1.0f);
		
	this->vertices.push_back(dwr_vertex);this->vertices.push_back(dwn_vertex);
	this->vertices.push_back(nxt_vertex);this->vertices.push_back(cur_vertex);

	this->vertex_indices.push_back(this->vertices.size() - 4);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 3);

	this->vertex_indices.push_back(this->vertices.size() - 3);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 1);

	cur_vertex.normal = -cur_vertex.normal;dwn_vertex.normal = -dwn_vertex.normal;
	dwr_vertex.normal = -dwr_vertex.normal;nxt_vertex.normal = -nxt_vertex.normal;

	this->vertices.push_back(dwr_vertex);this->vertices.push_back(dwn_vertex);
	this->vertices.push_back(nxt_vertex);this->vertices.push_back(cur_vertex);

	this->vertex_indices.push_back(this->vertices.size() - 1);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 3);

	this->vertex_indices.push_back(this->vertices.size() - 3);
	this->vertex_indices.push_back(this->vertices.size() - 2);
	this->vertex_indices.push_back(this->vertices.size() - 4);

	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->vertices.size() * sizeof(VertexAttributes), &this->vertices[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttributes), (GLvoid *) (sizeof(vec3) * 3));

	//	Each of the attributes to be used must be enabled.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Screen::Initialize - on exit"))
		return false;

	return true;
}

void Screen::TakeDown()
{
	super::TakeDown();
}

void Screen::Draw(const mat4 & projection, mat4 modelview, const ivec2 & size, const float time)
{
	if (this->GLReturnedError("Screen::Draw - on entry"))
		return;

	//modelview = rotate(modelview, time * 90.0f, vec3(0.0f, 1.0f, 0.0f));
	mat4 mvp = projection * modelview;

	this->shaders[this->shader_index]->Use();
	TextureManager::Inst()->BindTexture(texID, 0);
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(transpose(inverse(modelview))));
	this->shaders[this->shader_index]->CustomSetup();

	glBindVertexArray(this->vertex_array_handle);
#ifdef DRAW_POINTS
	glDrawArrays(GL_POINTS, 0, this->vertex_indices.size());
#else
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
#endif

	glUseProgram(0);
	glBindVertexArray(0);

	if (this->GLReturnedError("Screen::Draw - on exit"))
		return;
}
