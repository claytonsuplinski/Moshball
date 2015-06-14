/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Jumbotron shader - Same as texture shader except it 
is independent of the time of day. Allows it to be 
seen at night and in fog.  
*/
#version 400

in vec3 Position;
in vec3 Normal;
const vec4 LightPosition = vec4(0.0, 0.0, 0.0, 0.0);
const vec3 LightIntensity = vec3(0.85, 0.85, 0.85);
const vec3 Kd = vec3(0.6, 0.6, 0.6); // Diffuse reflectivity
const vec3 Ka = vec3(0.85, 0.85, 0.85); // Ambient reflectivity
const vec3 Ks = vec3(0.75, 0.75, 0.75); // Specular reflectivity
const float Shininess = 6.0f; // Specular shininess factor
layout (location = 0) out vec4 FragColor;

in vec2 tc;
uniform sampler2D picture;
uniform ivec2 size;

vec4 color = texture(picture, tc);
float factor = 1.0f;

const float blurSizeH = 1.0 / 300.0;const float blurSizeV = 1.0 / 200.0;

vec3 ads(){
vec3 n = normalize( Normal );
vec3 s = normalize( vec3(LightPosition) - Position );
vec3 v = normalize(vec3(-Position));
vec3 r = reflect( -s, n );
return 
LightIntensity * factor *
( Ka * vec3(color) +
Kd * vec3(color) * max( dot(s, n), 0.0 ));
}

void main()
{
	FragColor = vec4(ads(), 1.0);
}

