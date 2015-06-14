#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 projection_matrix;
uniform mat4 mvp;
uniform float time;

out vec3 Position;
out vec3 Normal;

void main()
{
	vec3 pos = vertex_position;
	if(time > 0){
		pos = (1 + time/15) * pos;
	}
	gl_Position = mvp * vec4(pos, 1.0);
}