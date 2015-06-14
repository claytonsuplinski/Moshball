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

vec4 color = texture(picture, tc);

const float blurSizeH = 1.0 / 300.0;
const float blurSizeV = 1.0 / 200.0;


vec3 ads()
{
vec3 n = normalize( Normal );
vec3 s = normalize( vec3(LightPosition) - Position );
vec3 v = normalize(vec3(-Position));
vec3 r = reflect( -s, n );
float dx = Position.x - 0.5;
float dy = Position.y - 0.5;
float dist = sqrt(dx * dx + dy * dy);
vec3 color2 =
mix( vec3(color.x, color.y, color.z), vec3(0.0,0.0,0.0),
smoothstep( 0.2, 4.0, dist )
);
return
LightIntensity *
( Ka * color2 +
Kd * color2 * max( dot(s, n), 0.0 ));
}

void main()
{
//Blurring
	/*vec4 sum = vec4( 0.0 );
    for (int x = -4; x <= 4; x++)
        for (int y = -4; y <= 4; y++)
            sum += texture(
                picture,
                vec2( tc.x + x * blurSizeH, tc.y + y * blurSizeV )
            )/ 81.0;
			
			FragColor = sum;
			*/

	
	FragColor = vec4(ads(), 1.0f);
}

