/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Sphere shader - Cool dynamically generated
design for the sphere; operates as a time-based color 
manipulation into a design. Phong shading is 
also included. Independent of time of day/environment
as the balls "light up".  
*/
#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 vertex_color;

uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 projection_matrix;
uniform mat4 mvp;
uniform float time;

out vec3 Position;
out vec3 Normal;
out vec3 color;

void main()
{
	vec3 pos = vertex_position;
	/*Special relativistic length contraction (didn't look very good)
	if(time > 0){
		pos = (1 + time/15) * pos;
	}*/
	color = vertex_color;
	Normal = normalize( normal_matrix * VertexNormal);
	Position = vec3( modelview_matrix * vec4(vertex_position,1.0) );
	gl_Position = mvp * vec4(pos, 1.0);
}