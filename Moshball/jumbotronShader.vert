/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Jumbotron shader - Same as texture shader except it 
is independent of the time of day. Allows it to be 
seen at night and in fog.  
*/
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec2 texture_coordinates;

uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 mvp;
uniform float time;

out vec3 Position;
out vec3 Normal;
out vec2 tc;

void main()
{
	Normal = normalize( normal_matrix * VertexNormal);
	Position = vec3( modelview_matrix * vec4(vertex_position,1.0) );
	tc = texture_coordinates;
	vec3 pos = vertex_position;
	gl_Position = mvp * vec4(pos, 1.0);
}