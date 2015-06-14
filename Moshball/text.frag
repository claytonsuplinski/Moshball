/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Text shader - Places the text on 
the screen. 
*/
varying vec2 texpos;
uniform sampler2D tex;
uniform vec4 color;

void main(void) {
  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;
}
