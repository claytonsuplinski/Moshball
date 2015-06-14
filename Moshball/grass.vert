#version 400
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec3 vertex_color;
out vec3 color;
out vec3 Position;
out vec3 Normal;
out float Time;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 projection_matrix;
uniform mat4 mvp;
uniform float time;
void main()
{
Time = time;
color = vertex_color;
Normal = normalize( normal_matrix * VertexNormal);
Position = vec3( modelview_matrix * vec4(VertexPosition,1.0) );
gl_Position = mvp * vec4(VertexPosition,1.0);
}
/*
#version 330 core
 
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
 
// Output data ; will be interpolated for each fragment.
out vec2 UV;
 
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
 
void main(){
 
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
 
    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}*/