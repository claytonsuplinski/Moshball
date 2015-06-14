#version 400

layout (location = 0) out vec4 FragColor;

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
#define PROCESSING_COLOR_SHADER

uniform float time;
vec2 mouse = vec2(0.0f, 0.0f);
vec2 resolution = vec2(100.0f, 100.0f);

float etoile(vec2 p){
	vec2 ap = abs(p);
	float retour=ap.x + ap.y;
	if(ap.x <=2.0*ap.y && ap.y<=ap.x) retour=1.5*ap.x; 
	if(2.0*ap.x > ap.y  && ap.y>=ap.x) retour=1.5*ap.y; 
	return retour;
}

void main( void ) {
	float a=time/1000.0;
	vec2 souris=vec2(-1.0+cos(a),-1.0+sin(a));
	vec2 position = ( gl_FragCoord.xy / resolution.xy * 2.0) +0.5*souris.xy;
	position *=6.0;
	mat2  rtz=mat2(cos(a),-sin(a),sin(a),cos(a));
	vec2 pos=rtz*position;
	float d = etoile(pos);
	d=mod(d,2.0);
	FragColor = vec4(d,0,0,1);
	
}