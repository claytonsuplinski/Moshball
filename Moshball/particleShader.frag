#version 400
in vec3 color;
in vec3 Position;
in vec3 Normal;
const vec4 LightPosition = vec4(0.0, 0.0, 0.0, 1.0);
const vec3 LightIntensity = vec3(0.85, 0.85, 0.85);
const vec3 Kd = vec3(0.1, 0.1, 0.1); // Diffuse reflectivity
const vec3 Ka = vec3(0.85, 0.85, 0.85); // Ambient reflectivity
const vec3 Ks = vec3(0.75, 0.75, 0.75); // Specular reflectivity
const float Shininess = 6.0f; // Specular shininess factor
layout( location = 0 ) out vec4 FragColor;
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
void main() {
//float dist = abs( Position.z );
//float fogFactor = (60.0f - dist) /
(60.0f - 35.0f);
//fogFactor = clamp( fogFactor, 0.0, 1.0 );
float dx = Position.x;
float dy = Position.y;
float dist = sqrt(dx * dx + dy * dy);
vec4 color2 =
mix( vec4(ads(), 1.0), vec4(ads(),0.0),
smoothstep( 0.5, 1.0, dist )
);
FragColor = color2;
}