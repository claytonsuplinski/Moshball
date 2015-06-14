/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Text shader - Places the text on 
the screen. 
*/
attribute vec4 coord;
varying vec2 texpos;

void main(void) {
  gl_Position = vec4(coord.xy, 0, 1);
  texpos = coord.zw;
}
