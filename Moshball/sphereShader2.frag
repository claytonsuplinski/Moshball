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

layout (location = 0) out vec4 FragColor;

in vec3 color;
in vec3 Position;
in vec3 Normal;
const vec4 LightPosition = vec4(0.0, 0.0, 0.0, 1.0);
const vec3 LightIntensity = vec3(1.85, 1.85, 1.85);
const vec3 Kd = vec3(0.1, 0.1, 0.1); // Diffuse reflectivity
const vec3 Ka = vec3(0.85, 0.85, 0.85); // Ambient reflectivity
const vec3 Ks = vec3(1.75, 1.75, 1.75); // Specular reflectivity
const float Shininess = 6.0f; // Specular shininess factor
// http://glsl.heroku.com/e#6541.0
//
// @paulofalcao
//
// Blue Pattern
//
// A old shader i had lying around
// Although it's really simple, I like the effect :)
// modified by @hintz


#ifdef GL_ES
precision mediump float;
#endif

// modified by @hintz

uniform float time;
uniform vec2 mouse = vec2(0.0f, 0.0f);
uniform vec2 resolution = vec2(40.0f, 40.0f);

#define PI 3.14159
#define TWO_PI (PI*2.0)
#define N 6.0

vec3 ads( )
{
vec3 n = normalize( Normal );
vec3 s = normalize( vec3(LightPosition) - Position );
vec3 v = normalize(vec3(-Position));
vec3 r = reflect( -s, n );
return
LightIntensity *
( Ka * color +
Kd * color * max( dot(s, n), 0.0 ) +
Ks * color * pow( max( dot(r,v), 0.0 ), Shininess ) );
}

void main(void) 
{
	vec2 center = (gl_FragCoord.xy);
	center.x=-100.12*sin(time/200.0);center.y=-100.12*cos(time/200.0);
	
	vec2 v = (gl_FragCoord.xy - resolution/20.0) / min(resolution.y,resolution.x) * 15.0;
	v.x=v.x-10.0;v.y=v.y-200.0;
	float col = 0.3;

	for(float i = 0.0; i < N; i++){
	  	float a = i * (TWO_PI/N) * 61.95;
		col += cos(TWO_PI*(v.y * cos(a) + v.x * sin(a) /*+ mouse.y +i*mouse.x*/ + sin(time*0.004)*100.0 ));
	}
	
	col /= 3.0;

	//c*0.1 + vec4(ads()*0.35, 1.0);
	FragColor = 0.25*vec4(col*1.0, 5*col*1.0, 0.0, 1.0) + vec4(ads()*0.65, 1.0) ;
}