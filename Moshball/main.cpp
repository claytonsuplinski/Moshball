/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Moshball - This video game has several modes.
See the README for the complete write-up about the 
game.
*/

#include <iostream>
#include <assert.h>
#include <vector>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <conio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader_utils.h"

#include "disc.h"
#include "sphere.h"
#include "cylinder.h"
#include "torus.h"
#include "square.h"
#include "tiger.h"
#include "squareStadium.h"
#include "discStadium.h"
#include "sphereStadium.h"
#include "coin.h"
#include "alien.h"
#include "shark.h"
#include "triangle.h"
#include "bird.h"
#include "speedBoost.h"
#include "gengar.h"
#include "cube2.h"
#include "sphere2.h"
#include "square2.h"
#include "square4.h"

#include "vertexattributes2.h"
#include "object2.h"
#include "square2.h"
#include "shader2.h"
#include "globals.h"
#include "fbo.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

using namespace std;using namespace glm;

//x (0=Normal, 1=night, 2=twilight), y(0=normal, 1=underwater, 2=fog)
ivec2 tod = ivec2(0, 0);

//Creates a class for maintaining the window information for easy access.
class Window
{
public:
	Window()
	{
		this->time_last_pause_began = this->total_time_paused = 0;
		this->normals = this->wireframe = this->paused = false;
		this->slices = 20;this->interval = 1000 / 120;this->window_handle = -1;
	}
	float time_last_pause_began;float total_time_paused;bool paused , wireframe, normals;
	int window_handle;int interval;int slices;ivec2 size;float window_aspect;
	vector<string> instructions, countdown, scores;
} window;

/////////////////////////////////////VARIABLE DECLARATIONS/////////////////////////////
//Menu decision variables
bool menuOn = true;int menuSelect = 0;bool singlePlayer = false;
bool twoPlayer = false;bool secondPlayerTurn = false;int stadiumSelect = 0;
bool weatherSelect = false;

//Values defining the position/rotation of the camera.
double RotatedX = 8;double RotatedY = 0;float transX = 0;
float transY = 0;float transZ = 0;

float velocity = 1; //Define the travel of the user
int score = 0; //The user's score for the game
bool caught = false; //If the enemy caught the user
float firstPersonTime, firstPersonScore; //Variables used for the 2 player game
bool startupTime = true; //Account for delay in loading the music
int gameSelect = 0; //Select amongst the game types
int viewPerspective = 0; //Select amongst 1st and 3rd person views
int weatherType = 0; //Select amongst the weather types

//Used for adjusting the y value for the lookAt function for the first and third person views.
float perspectiveOffset = 0.0f;
//Used for adjusting the z value for the lookAt function for the first and third person views.
float perspectiveStepBack = 50.0f;

bool countdownOn = false; //If the menu is off and the countdown is on
bool won = false; //If the user won
float* partX, *partY, *partOffset;int numParts; //Variables controlling the precipitation.

//Time control variables.
float current_timeDisplay, menuTime, countdownTime; 
string timeString;int first = 1;int menuFirst = 1;

//The projection and modelview matrices. 
mat4 projection, modelview;

//Mouse value/identification variables.
float mouseX = 0;float mouseY = 0;
float prevMouseY = (float)(window.size.y/2);

float maxSpeed = 1.6f; //Maximum velocity for user

//////////////////////////////////////TEXT RENDERING/////////////////////////////////////////////
////////Font variables//////////
GLuint program;GLint attribute_coord;GLint uniform_tex;GLint uniform_color;
struct point {GLfloat x;GLfloat y;GLfloat s;GLfloat t;};
GLuint vbo;FT_Library ft;FT_Face face;const char *fontfilename;
int init_resources()
{        /* Initialize the FreeType2 library */
        if(FT_Init_FreeType(&ft)){fprintf(stderr, "Could not init freetype library\n");return 0;}
        /* Load a font */
        if(FT_New_Face(ft, fontfilename, 0, &face)){fprintf(stderr, "Could not open font %s\n", fontfilename);return 0;}
        /* Compile and link the shader program */
        GLint link_ok = GL_FALSE;GLuint vs, fs;
        if ((vs = create_shader("text.vert", GL_VERTEX_SHADER))        == 0) return 0;
        if ((fs = create_shader("text.frag", GL_FRAGMENT_SHADER)) == 0) return 0;

        program = glCreateProgram();glAttachShader(program, vs);glAttachShader(program, fs);
        glLinkProgram(program);glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
        if (!link_ok) {fprintf(stderr, "glLinkProgram:");print_log(program);return 0;}

        const char* attribute_name;attribute_name = "coord";
        attribute_coord = glGetAttribLocation(program, attribute_name);
        if (attribute_coord == -1) {fprintf(stderr, "Could not bind attribute %s\n", attribute_name);return 0;}

        const char* uniform_name;uniform_name = "tex";
        uniform_tex = glGetUniformLocation(program, uniform_name);
        if (uniform_tex == -1){fprintf(stderr, "Could not bind uniform %s\n", uniform_name);return 0;}

        uniform_name = "color";uniform_color = glGetUniformLocation(program, uniform_name);
        if (uniform_color == -1){fprintf(stderr, "Could not bind uniform %s\n", uniform_name);return 0;}

        // Create the vertex buffer object
        glGenBuffers(1, &vbo);
        return 1;
}

/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
void render_text(const char *text, float x, float y, float sx, float sy) {
        const char *p;FT_GlyphSlot g = face->glyph;

        /* Create a texture that will be used to hold one "glyph" */
        GLuint tex;glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &tex);glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(uniform_tex, 0);
        /* We require 1 byte alignment when uploading texture data */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /* Set up the VBO for our vertex data */
        glEnableVertexAttribArray(attribute_coord);glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
        /* Loop through all characters */
        for(p = text; *p; p++) {
                /* Try to load and render the character */
                if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
                        continue;

                /* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
                glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
                /* Calculate the vertex and texture coordinates */
                float x2 = x + g->bitmap_left * sx;float y2 = -y - g->bitmap_top * sy;
                float w = g->bitmap.width * sx;float h = g->bitmap.rows * sy;

                point box[4] = {
                        {x2,     -y2    , 0, 0}, {x2 + w, -y2    , 1, 0},
                        {x2,     -y2 - h, 0, 1}, {x2 + w, -y2 - h, 1, 1},
                };

                /* Draw the character on the screen */
                glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                /* Advance the cursor to the start of the next character */
                x += (g->advance.x >> 6) * sx;y += (g->advance.y >> 6) * sy;
        }
        glDisableVertexAttribArray(attribute_coord);glDeleteTextures(1, &tex);
}

void textDisplay(int textIndex, float windowX, float windowY, int timeOfDay, bool pokemon)
{
		glDisable(GL_CULL_FACE);
        float sx = (float)(2.0/windowX);
        float sy = (float)(2.0/windowY);

        glUseProgram(program);

        glClearColor(1.0,1.0,0.25,1);
       // glClear(GL_COLOR_BUFFER_BIT);

        /* Enable blending, necessary for our alpha texture */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GLfloat realRed[4] = {1, 0, 0, 1.0};
        GLfloat red[4] = {1, 1, 0, 1};
        GLfloat transparent_green[4] = {0, 1, 0, 0.5};

        /* Set font size to 48 pixels, color to black */
        FT_Set_Pixel_Sizes(face, 0, 48);
		
		if(textIndex == 0){
			glUniform4fv(uniform_color, 1, realRed);
			render_text("Moshball - Main Menu",            -1 + 8 * sx,   1 - 50 * sy,    sx, sy);
			glUniform4fv(uniform_color, 1, red);
			render_text("(1) Tiger Echo Plateau",            -1 + 8 * sx,   1 - 150 * sy,    sx, sy);
			render_text("(2) The Intergalactic Dome",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);
			render_text("(3) Marine Layer Arena",            -1 + 8 * sx,   1 - 250 * sy,   sx, sy);
			render_text("(4) Weather Select",            -1 + 8 * sx,   1 - 300 * sy,   sx, sy);
			render_text("(b) Back",            -1 + 8 * sx,   1 - 350 * sy,   sx, sy);
		}
		else if(textIndex == 1){
			glUniform4fv(uniform_color, 1, realRed);
			render_text("Moshball - Weather Select",            -1 + 8 * sx,   1 - 50 * sy,   sx, sy);
			glUniform4fv(uniform_color, 1, red);
			render_text("(1) No weather",            -1 + 8 * sx,   1 - 150 * sy,   sx, sy);
			render_text("(2) Fog",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);
			render_text("(3) Rain",            -1 + 8 * sx,   1 - 250 * sy,   sx, sy);
			render_text("(4) Snow",            -1 + 8 * sx,   1 - 300 * sy,   sx, sy);
			render_text("(b) Back",            -1 + 8 * sx,   1 - 350 * sy,   sx, sy);
		}
		else if(textIndex == 2){
			glUniform4fv(uniform_color, 1, realRed);
			render_text("Moshball - Player Options",            -1 + 8 * sx,   1 - 50 * sy,   sx, sy);
			glUniform4fv(uniform_color, 1, red);
			render_text("(1) Single Player",            -1 + 8 * sx,   1 - 150 * sy,   sx, sy);
			render_text("(2) Two Players (Not implemented)",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);
			render_text("(b) Back",            -1 + 8 * sx,   1 - 250 * sy,   sx, sy);
		}
		else if(textIndex == 3 || textIndex == 4 || textIndex == 5){
			glUniform4fv(uniform_color, 1, realRed);
			render_text("Moshball - Game Type Select",            -1 + 8 * sx,   1 - 50 * sy,   sx, sy);
			glUniform4fv(uniform_color, 1, red);
			render_text("(1) Moshball",            -1 + 8 * sx,   1 - 150 * sy,   sx, sy);	
			if(textIndex == 3){
				if(pokemon){render_text("(2) Avoid the Gengar",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);}
				else{render_text("(2) Avoid the Tiger",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);}}
			else if(textIndex == 4){
				if(pokemon){render_text("(2) Avoid the Magneton",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);}
				else{render_text("(2) Avoid the Alien",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);}}
			else if(textIndex == 5){
				if(pokemon){render_text("(2) Avoid the Tentacool",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);}
				else{render_text("(2) Avoid the Shark",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);}}
			render_text("(3) Coin Time Attack",            -1 + 8 * sx,   1 - 250 * sy,   sx, sy);
			render_text("(b) Back",            -1 + 8 * sx,   1 - 300 * sy,   sx, sy);
		}
		else if(textIndex == 6){
			glUniform4fv(uniform_color, 1, realRed);
			render_text("Moshball - View Perspective Select",            -1 + 8 * sx,   1 - 50 * sy,   sx, sy);
			glUniform4fv(uniform_color, 1, red);
			render_text("(1) First person view",            -1 + 8 * sx,   1 - 150 * sy,   sx, sy);
			render_text("(2) Third person view",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);
			render_text("(b) Back",            -1 + 8 * sx,   1 - 250 * sy,   sx, sy);
		}
		else if(textIndex == 7){
			glUniform4fv(uniform_color, 1, realRed);
			render_text("Moshball - Game Type Select",            -1 + 8 * sx,   1 - 50 * sy,   sx, sy);
			glUniform4fv(uniform_color, 1, red);
			render_text("(1) Moshball (Timed)",            -1 + 8 * sx,   1 - 150 * sy,   sx, sy);
			render_text("(2) Coin Collection",            -1 + 8 * sx,   1 - 200 * sy,   sx, sy);
			render_text("(b) Back",            -1 + 8 * sx,   1 - 250 * sy,   sx, sy);
		}
		else if(textIndex == 8){glUniform4fv(uniform_color, 1, realRed);render_text("Get Ready!",            -0.2f,  0,   sx, sy);}
		glUniform4fv(uniform_color, 1, red);
		if(timeOfDay == 0){render_text("(5) Daytime",      -1 + 8 * sx,   1 - 400 * sy,   sx, sy);}
		else if(timeOfDay == 1){render_text("(5) Night",      -1 + 8 * sx,   1 - 400 * sy,   sx, sy);}
		else if(timeOfDay == 2){render_text("(5) Twilight",      -1 + 8 * sx,   1 - 400 * sy,   sx, sy);}

		if(pokemon){render_text("(6) vs Pokemon",      -1 + 8 * sx,   1 - 450 * sy,   sx, sy);}
		else{render_text("(6) No Pokemon",      -1 + 8 * sx,   1 - 450 * sy,   sx, sy);}
		
		glDisable(GL_BLEND);
		glUseProgram(0);
		glEnable(GL_CULL_FACE);
		
		
}

void activeTextDisplay(const char* words, float x, float y)
{
		glDisable(GL_CULL_FACE);
        float sx = (float)(2.0/glutGet(GLUT_WINDOW_WIDTH));
        float sy = (float)(2.0/glutGet(GLUT_WINDOW_HEIGHT));

        glUseProgram(program);

        /* Enable blending, necessary for our alpha texture */
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		/* Set font size to 48 pixels, color to black */
        FT_Set_Pixel_Sizes(face, 0, 36);

		if(weatherType != 3 || tod.x == 1){GLfloat black[4] = {1, 1, 1, 1};glUniform4fv(uniform_color, 1, black);}
		else{GLfloat black[4] = {0,0,0,0.75};glUniform4fv(uniform_color, 1, black);}
		
		render_text(words,x,y,sx,sy); 
		
		glDisable(GL_BLEND);
		glUseProgram(0);
        //glFinish();
		glEnable(GL_CULL_FACE);
}

/////////////////////////////////////AUDIO CONTROLS/////////////////////////////////////////////////////
 // Buffers to hold sound data.
ALuint Buffer;ALuint origBuffer;
// Sources are points of emitting sound.
ALuint Source;ALuint origSource;
/*
 * These are 3D cartesian vector coordinates. A structure or class would be
 * a more flexible of handling these, but for the sake of simplicity we will
 * just leave it as is.
 */
// Position of the source sound.
ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
// Velocity of the source sound.
ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
// Position of the Listener.
ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
// Velocity of the Listener.
ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
// Also note that these should be units of '1'.
ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

/*
 * ALboolean LoadALData()
 *
 *         This function will load our sample data from the disk using the Alut
 *         utility and send the data into OpenAL as a buffer. A source is then
 *         also created to play that buffer.
 */
ALboolean LoadALData(ALbyte *song, bool looping){
            // Variables to load into.
	        ALenum format;ALsizei size;ALvoid* data;ALsizei freq;
            ALboolean loop = true; 

            // Load wav data into a buffer.
            alGenBuffers(1, &Buffer);
			if(alGetError() != AL_NO_ERROR)
                        return AL_FALSE;

    // Load any of your favourite wav song here
            alutLoadWAVFile(song, &format, &data, &size, &freq, &loop);
            alBufferData(Buffer, format, data, size, freq);
            alutUnloadWAV(format, data, size, freq);
			
            // Bind the buffer with the source.
            alGenSources(1, &Source);
            if(alGetError() != AL_NO_ERROR)
                        return AL_FALSE;
			
            alSourcei (Source, AL_BUFFER,   Buffer   );
            alSourcef (Source, AL_PITCH,    1.0      );
            alSourcef (Source, AL_GAIN,     1.0      );
            alSourcefv(Source, AL_POSITION, SourcePos);
            alSourcefv(Source, AL_VELOCITY, SourceVel);
			if(looping){
            alSourcei (Source, AL_LOOPING,  AL_TRUE     );
			}
			else{
			alSourcei (Source, AL_LOOPING,  AL_FALSE     );
			}

            // Do another error check and return.
            if(alGetError() == AL_NO_ERROR)
                        return AL_TRUE;
			
            return AL_FALSE;
}

/*
 * void SetListenerValues()
 *
 *         We already defined certain values for the Listener, but we need
 *         to tell OpenAL to use that data. This function does just that.
 */
void SetListenerValues(){
            alListenerfv(AL_POSITION,    ListenerPos);alListenerfv(AL_VELOCITY,    ListenerVel);
            alListenerfv(AL_ORIENTATION, ListenerOri);
}

/*
 * void KillALData()
 *
 *         We have allocated memory for our buffers and sources which needs
 *         to be returned to the system. This function frees that memory.
 */
void KillALData(){/*alDeleteBuffers(1, &Buffer);alDeleteSources(1, &Source);alDeleteBuffers(1, &origBuffer);alDeleteSources(1, &origSource);*/alutExit();}

void init(ALbyte *song, bool loop){
            // Initialize OpenAL and clear the error bit. 
            alutInit(NULL, 0);alGetError();
			
            // Load the wav data. 
            if(LoadALData(song, loop) == AL_FALSE) //method 1: LoadALData()
            {
                printf("Error loading data.");
				return;
            }
			
            SetListenerValues(); //method 2: void SetListenerValues()
			
            // Setup an exit procedure. 
            atexit(KillALData); //method 3: void KillALData()
			if(loop){origSource = Source;}
			alSourcePlay(Source);

}

////////////////////////////////////OBJECT CREATION////////////////////////////////////
Sphere sphere, sph1, enm, usr;Sphere sphere2;Cylinder cylinder;Disc disc; 
Torus torus;Square square;Square square2;Tiger tiger;Sphere rain2;Sphere snow2;
SquareStadium stadium, stadiumRain, stadiumWinter;DiscStadium stadium1;SphereStadium stadium2;
Coin coin;Alien alien;Shark shark;Bird bird;SpeedBoost sb;Square4 sq4;
Gengar gengar, magneton, haunter, star, frog, goldeen;Square2 tri2;FrameBufferObject fbo;
Cube2 skybox, skybox2, skybox3, skyboxUW;Sphere2 egg;

////////////////////////////////////MOUSE DETECTION & MANAGEMENT/////////////////////////
void mouseMovement(int x, int y) {
    mouseX=(float)x; //set lastx to the current x position
    mouseY=(float)y; //set lasty to the current y position
}

void mouseRotations(int stadium, int person){
	if(person == 1 || person == 3){
		//Turning controls
		if(mouseX < window.size.x/2){RotatedY -= abs((mouseX - window.size.x/2)/75);}
		else if(mouseX > window.size.x/2){RotatedY += abs((mouseX - window.size.x/2)/75);}
		//Forward & Backward Velocity controls
		if(mouseY < window.size.y/2 && velocity < maxSpeed*(2*abs(mouseY-(window.size.y/2))/window.size.y)){
			velocity += abs((mouseY - window.size.y/2)/1000);
		}
		else if(mouseY < window.size.y/2 && mouseY > prevMouseY){velocity += -abs((mouseY - window.size.y/2)/1000);}
		if(mouseY > window.size.y/2 && velocity > -maxSpeed*(2*abs(mouseY-(window.size.y/2))/window.size.y)){
			velocity += -abs((mouseY - window.size.y/2)/1000);
		}
		else if(mouseY > window.size.y/2 && mouseY < prevMouseY){
			velocity += abs((mouseY - window.size.y/2)/1000);
		}
	}
	if(stadium > 2){   //Up/Down Turning Controls
		if(mouseY < window.size.y/2 && RotatedX >= -89){
			RotatedX -= abs((mouseY - window.size.y/2)/75);
		}
		else if(mouseY > window.size.y/2 && RotatedX <= 89){
			RotatedX += abs((mouseY - window.size.y/2)/75);
		}
	}
	prevMouseY = mouseY;
}

/////////////////////////////////////CAMERA CONTROLS//////////////////////////////////////////
//Moves the camera according to user inputs
glm::mat4 render(glm::mat4 Model){	
	//Collision with a flat wall
	if(stadiumSelect == 1){
		if(transX > 50.8){
			transX = 50.8f;RotatedY = RotatedY + 2*(0 - RotatedY);	
		}
		if(transX < -52.8){
			transX = -52.8f;RotatedY = RotatedY + 2*(180 - RotatedY);	
		}
		if(transZ > 51.8){
			transZ = 51.8f;RotatedY = RotatedY + 2*(90 - RotatedY);
		}
		if(transZ < -51.8){
			transZ = -51.8f;RotatedY = RotatedY + 2*(270 - RotatedY);
		}
	}
	
	Model = glm::rotate(Model, (GLfloat) RotatedX, glm::vec3(1,0,0));
	Model = glm::rotate(Model, (GLfloat) RotatedY, glm::vec3(0,1,0));
	Model = glm::translate(Model, glm::vec3(transX,0,0));    
	Model = glm::translate(Model, glm::vec3(0,0,transZ));    
	Model = glm::translate(Model, glm::vec3(0,transY,0));	
	glLoadMatrixf(glm::value_ptr(Model));
    return Model;
}

//Undoes the render function. 
glm::mat4 endRender(glm::mat4 Model){	
	Model = glm::translate(Model, glm::vec3(0,-transY,0));	
	Model = glm::translate(Model, glm::vec3(0,0,-transZ));
	Model = glm::translate(Model, glm::vec3(-transX,0,0));
	Model = glm::rotate(Model, (GLfloat) -RotatedY, glm::vec3(0,1,0));
	Model = glm::rotate(Model, (GLfloat) -RotatedX, glm::vec3(1,0,0));			
	glLoadMatrixf(glm::value_ptr(Model));
    return Model;
}

//Changes the value of the angle in which the camera should be rotated vertically.
void rotateX(double angle){
    if(!(RotatedX>=89 && angle>0) && !(RotatedX<=-89 && angle<0)){
        RotatedX += angle;
    }
}

//Changes the value of the angle in which the camera should be rotated horizontally.
void rotateY(double angle){
    RotatedY += angle;
}

//Coin variables
int numCoins = 20;
float* coinX = new float[numCoins];float* coinY = new float[numCoins];float* coinZ = new float[numCoins];

//Sphere variables
int numSpheres = 50;  
float* sphereX = new float[numSpheres];float* sphereY = new float[numSpheres];float* sphereZ = new float[numSpheres];
int* leavingSpheres = new int[numSpheres];int* sphereTime = new int[numSpheres];float* hit = new float[numSpheres];

//Enemy variables
float enemyX, enemyY, enemyZ;bool vsPokemon = true;

//Speedboost variables
float sbX, sbZ;

//Number of targets remaining
int targets = numSpheres;

//Initialize the enemy's location
void initEnemy(){enemyX = 30;enemyY = 30;enemyZ = 30;}

//Draw the enemy always facing the user
void makeEnemy(mat4 modelview, mat4 projection){
	if(gameSelect != 3){
		mat4 another;
		float t = float(glutGet(GLUT_ELAPSED_TIME)) / 70000.0f;
		if(!caught){
			if(enemyX > transX+0.5){enemyX -= (float)(t*0.2);}
			else if(enemyX < transX-0.5){enemyX += (float)(t*0.2);}
			if(enemyZ > transZ+0.5){enemyZ -= (float)(t*0.2);}
			else if(enemyZ < transZ-0.5){enemyZ += (float)(t*0.2);}
			if(stadiumSelect == 3){
				if(enemyY > transY+0.5){enemyY -= (float)(t*0.1);}
				else if(enemyY < transY-0.5){enemyY += (float)(t*0.1);}
			}
		}
		float xSide = enemyX - transX;float zSide = enemyZ - transZ;
		float diagonal = sqrt(xSide*xSide + zSide*zSide);
		float angle = acos(xSide/diagonal);
		angle = (float)(180*angle/3.141);
		if(zSide <=0){angle = -angle;}
		if(stadiumSelect == 1){
			if(vsPokemon){//Gengar or the tiger
				another = translate(modelview, vec3(-enemyX,-0.75,-enemyZ));
				another = rotate(another, -angle + 90.0f, vec3(0,1,0));
				mat4 scaler;
				scaler = another;
				scaler = scale(scaler, vec3(0.01, 0.01, 0.01));
				gengar.Draw(projection, scaler, tod, velocity);
			}
			else{
				another = translate(modelview, vec3(-enemyX,1.1,-enemyZ));
				another = rotate(another, 270.0f, vec3(1,0,0));
				another = rotate(another, 90.0f, vec3(0,1,0));
				another = rotate(another, angle + 270.0f, vec3(1,0,0));
				tiger.Draw(projection, another, tod, 0);
			}
		}
		else if(stadiumSelect == 2){
			if(vsPokemon){//Magneton or the alien
				another = translate(modelview, vec3(-enemyX,-0.75,-enemyZ));
				another = rotate(another, -angle + 90.0f, vec3(0,1,0));
				mat4 scaler;
				scaler = another;
				if(!caught){
					scaler = scale(scaler, vec3(0.05, 0.05, 0.05));
					magneton.Draw(projection, scaler, tod, 0);
				}
				else{
					float dance = float(glutGet(GLUT_ELAPSED_TIME)) / 150.0f;
					scaler = translate(scaler, vec3(abs(sin(dance)), 0.0f, 0.0f));
					scaler = scale(scaler, vec3(0.05, 0.05, 0.05));
					magneton.Draw(projection, scaler, tod, 0);
				}
			}
			else{
				another = translate(modelview, vec3(-enemyX,0.65,-enemyZ));
				another = rotate(another, 270.0f, vec3(1,0,0));
				another = rotate(another, 90.0f, vec3(0,1,0));
				another = rotate(another, angle + 270.0f, vec3(1,0,0));
				if(!caught){
					alien.Draw(projection, another, tod, 0);
				}
				else{
					alien.Draw(projection, another, tod, 1);
				}
			}
		}
		else if(stadiumSelect == 3){ //Tentacool or the shark
			float ySide = enemyY - transY;
			float diagonal2 = sqrt(xSide*xSide + ySide*ySide);
			float angle2 = acos(xSide/diagonal2);
			angle2 = (float)(180*angle2/3.141);
			if(ySide <=0){angle2 = -angle2;}
			if(vsPokemon){
				another = translate(modelview, vec3(-enemyX,-enemyY-1.25,-enemyZ));
				another = rotate(another, -angle + 90.0f, vec3(0,1,0));
				mat4 scaler;
				scaler = another;
				scaler = scale(scaler, vec3(0.5, 0.5, 0.5));
				haunter.Draw(projection, scaler, tod, 0);
			}
			else{
				another = translate(modelview, vec3(-enemyX,-enemyY,-enemyZ)); 
				another = rotate(another, 270.0f, vec3(1,0,0));
				another = rotate(another, 90.0f, vec3(0,1,0));
				another = rotate(another, angle + 270.0f, vec3(1,0,0));
				shark.Draw(projection, another, tod, 0);
			}
		}
	}
	else{ //In time attack, game ends at 100 seconds regardless
		if(current_timeDisplay - menuTime - countdownTime >= 100){
			caught = true;
		}
	}
}

//Initialize the coin locations for square stadium
void initCoins(){
	float x, z;
	for(int i=0; i < numCoins; i++){
		x = (float)(rand() % 1006 - 518);x = x/10;
		z = (float)(rand() % 1006 - 518);z = z/10;
		coinX[i] = x;coinZ[i] = z;}
	x = (float)(rand() % 1006 - 518);x = x/10;
	z = (float)(rand() % 1006 - 518);z = z/10;
	sbX = x;sbZ = z;
}
//Initialize the coin locations for disc stadium
void initDiscCoins(){
	float x, r;
	for(int i=0; i < numCoins; i++){
		r = (float)(rand() % 518);r = r/10;
		x = (float)(rand() % 628);x = x/100;
		coinX[i] = r * cos(x);coinZ[i] = r * sin(x);}
	r = (float)(rand() % 518);r = r/10;
	x = (float)(rand() % 628);x = x/100;
	sbX = r * cos(x);sbZ = r * sin(x);
}
//Initialize the coin locations for sphere stadium
void initSphereCoins(){
	float x, z, r;
	for(int i=0; i < numCoins; i++){
		x = (float)(rand() % 628);x = x/100;
		z = (float)(rand() % 628);z = z/100;
		r = (float)(rand() % 523);r = r/10;

		coinX[i] = r* cos(x) * sin(z);
		coinZ[i] = r* sin(x) * sin(z);
		coinY[i] = r* cos(z);
	}
}
//Draw the coins and make them spin
void makeCoins(mat4 modelview, mat4 projection){
	mat4 another; float x, z, r, timeSpin;
	timeSpin = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
	if(stadiumSelect == 3){
		for(int i=0; i < numCoins; i++){
			if(coinX[i] == -1000){
				x = (float)(rand() % 628);x = x/100;
				z = (float)(rand() % 628);z = z/100;

				coinX[i] = (float)(51.8* cos(x) * sin(z));
				coinZ[i] = (float)(51.8* sin(x) * sin(z));
				coinY[i] = (float)(51.8* cos(z));
			}
			another = translate(modelview, vec3(coinX[i],coinY[i],coinZ[i]));
			coin.Draw(projection, another, tod, 0);
		}	
	}
	else{
		//Draw the speed boost
		if(sbX == -1000){
			if(stadiumSelect == 1){
				x = (float)(rand() % 1006 - 518);x = x/10;
				z = (float)(rand() % 1006 - 518);z = z/10;
				sbX = x;sbZ = z;
			}
			else if(stadiumSelect == 2){
				r = (float)(rand() % 518);
				r = r/10;
				x = (float)(rand() % 628);
				x = x/100;
				sbX = r * cos(x);sbZ = r * sin(x);
			}
		}
		another = translate(modelview, vec3(sbX,-0.75,sbZ));
		sb.Draw(projection, another, tod, 0);
	}
}

//Initialize the sphere locations for square stadium
void initSpheres(){
	float x, z;
	for(int i=0; i < numSpheres; i++){
		x = (float)(rand() % 1006 - 518);x = x/10;
		z = (float)(rand() % 1006 - 518);z = z/10;
		sphereX[i] = x;sphereZ[i] = z;
		hit[i] = 1;leavingSpheres[i] = 0;sphereTime[i] = 0;
	}
}
//Initialize the sphere locations for disc stadium
void initDiscSpheres(){
	float x, r;
	for(int i=0; i < numSpheres; i++){
		r = (float)(rand() % 518);r = r/10;
		x = (float)(rand() % 628);x = x/100;
		sphereX[i] = r * cos(x);sphereZ[i] = r * sin(x);
		hit[i] = 0;leavingSpheres[i] = 0;sphereTime[i] = 0;
	}
}
//Initialize the sphere locations for sphere stadium
void initSphereSpheres(){
	float x, z, r;
	for(int i=0; i < numSpheres; i++){
		x = (float)(rand() % 628);x = x/100;
		z = (float)(rand() % 628);z = z/100;
		r = (float)(rand() % 523);r = r/10;
		sphereX[i] = r* cos(x) * sin(z);sphereZ[i] = r* sin(x) * sin(z);
		sphereY[i] = r* cos(z);
		hit[i] = 0;leavingSpheres[i] = 0;sphereTime[i] = 0;
	}
}
//Draw the spheres and take collisions into account (but not calculate them)
void makeSpheres(mat4 modelview, mat4 projection){
	mat4 another;float x, z, r;
	if(stadiumSelect == 3){
		for(int i=0; i < numSpheres; i++){
			if(sphereX[i] == -1000){
				x = (float)(rand() % 628);x = x/100;
				z = (float)(rand() % 628);z = z/100;
				r = (float)(rand() % 523);r = r/10;
				sphereX[i] = r* cos(x) * sin(z);sphereZ[i] = r* sin(x) * sin(z);
				sphereY[i] = r* cos(z);
			}
			another = translate(modelview, vec3(sphereX[i],sphereY[i],sphereZ[i]));
			if(hit[i] == 0){sphere2.Draw(projection, another, tod, velocity);}
			else{sphere.Draw(projection, another, tod, velocity);}
		}	
	}
	else{
		for(int i=0; i < numSpheres; i++){
			if(sphereX[i] == -1000){
				if(stadiumSelect == 1){
					x = (float)(rand() % 1006 - 518);x = x/10;z = (float)(rand() % 1006 - 518);z = z/10;
					sphereX[i] = x;sphereZ[i] = z;
				}
				else if(stadiumSelect == 2){
					r = (float)(rand() % 518);r = r/10;x = (float)(rand() % 628);x = x/100;
					sphereX[i] = r * cos(x);sphereZ[i] = r * sin(x);
				}
			}
			another = translate(modelview, vec3(sphereX[i],0,sphereZ[i]));
			//If sphere isn't hit
			if(hit[i] == 0){
				//When the ball isn't supposed to be selected, make it the "off" color
				if(gameSelect == 1 && (sphereTime[i] >= current_timeDisplay || sphereTime[i] == 0)){
					sphere2.Draw(projection, another, tod, velocity);
				}
				else if(gameSelect == 1 && sphereTime[i] < current_timeDisplay){hit[i] = 1;}
				else{egg.Draw(projection, another, tod, 0);}	
			}
			//If sphere is hit
			else{
				//If sphere 
				if(gameSelect != 1){sphere.Draw(projection, another, tod, velocity);}
				else if(gameSelect == 1 && (sphereTime[i] >= current_timeDisplay || sphereTime[i] == 0)){
					sphere.Draw(projection, another, tod, velocity);}
				else if(gameSelect == 1 && sphereTime[i] < current_timeDisplay){hit[i] = 0;targets++;sphereTime[i] = 0;}
				else{sphere2.Draw(projection, another, tod, 0);
				}
				
			}
			/*//Attempt at putting text over ball
			mat4 textDisp = another;
			textDisp = translate(textDisp, vec3(0.0f, 2.0f, 0.0f));
			ostringstream convert;
			convert << sphereTime[i];      // insert the textual representation of 'Number' in the characters in the stream
			activeTextDisplay(convert.str().c_str(), 0.0f, 0.0f);*/
		}
	}	
}
//Do the correct thing when the user collides with a ball
void checkBallCollisions(){
	vec3 a;
	if(stadiumSelect == 3){
		for(int i=0; i < numSpheres; i++){
			a = vec3(sphereX[i],sphereY[i],sphereZ[i]) - vec3(-transX, -transY, -transZ);
			if(sqrt(dot(a, a)) < 2){
				if(hit[i] == 0){targets--;}
				if(gameSelect == 1){hit[i]=1;}
				score+=10;
				if(gameSelect != 1){sphereX[i] = -1000;}
			}			
		}	
	}
	else{
		for(int i=0; i < numSpheres; i++){
			a = vec3(sphereX[i],0,sphereZ[i]) - vec3(-transX, 0, -transZ); //Normal vector to ball
			if(sqrt(dot(a, a)) < 2){
				if(gameSelect == 1 && leavingSpheres[i] == 0){
					leavingSpheres[i] = 1;
					//Infinitesimal velocity vector of user on collision
					vec3 b = vec3(velocity*sin(-RotatedY*3.14/180),0,velocity*cos(-RotatedY*3.14/180));
					vec3 c = vec3(velocity*sin(-(RotatedY+1)*3.14/180),0,velocity*cos(-(RotatedY+1)*3.14/180));
					float angle = (float)(acos((dot(a,b)/(sqrt(dot(a, a)) * sqrt(dot(b, b)))))*180/3.14);
					float angle2 = (float)(acos((dot(a,b)/(sqrt(dot(a, a)) * sqrt(dot(b, b))))));
					float angle3 = (float)(acos((dot(a,c)/(sqrt(dot(a, a)) * sqrt(dot(c, c)))))*180/3.14);

					if(angle3 > angle){RotatedY -= angle * (angle - 90)/90;}
					else{RotatedY += angle * (angle - 90)/90;}

					if(hit[i] == 0){targets--;}
					hit[i] = 1; 
					sphereTime[i]= (int)(current_timeDisplay+30+numSpheres*2);
				}
				else{
					if(!(gameSelect == 1)){sphereX[i] = -1000;}
					if(hit[i] == 0){targets--;}
					hit[i] = 1;score+=10;
				}				
			}
			else{
				leavingSpheres[i] = 0;
			}
		}
	}	
}
//Collect a coin on collision
void checkCoinCollisions(){
	vec3 a;
	if(stadiumSelect == 3){
		for(int i=0; i < numCoins; i++){
			a = vec3(coinX[i],coinY[i],coinZ[i]) - vec3(-transX, -transY, -transZ);
			if(sqrt(dot(a, a)) < 2){
				coinX[i] = -1000;score += 50;
			}			
		}	
	}
	else{
		for(int i=0; i < numCoins; i++){
			a = vec3(coinX[i],0,coinZ[i]) - vec3(-transX, 0, -transZ);
			if(sqrt(dot(a, a)) < 2){
				coinX[i] = -1000;score += 50;
			}
		}
	}	
}
//Check whether the user collided with the enemy or speed boost
void checkEnemyCollision(){
	vec3 a;
	if(stadiumSelect < 3){
		a = vec3(-enemyX,0,-enemyZ) - vec3(-transX, 0, -transZ);
		if(sqrt(dot(a, a)) < 2){
			caught = true;
		}
		a = vec3(sbX,0,sbZ) - vec3(-transX, 0, -transZ);
		if(sqrt(dot(a, a)) < 2){
			sbX = -1000;
			//stadiumSelect = (stadiumSelect+1)%2 + 1;
			maxSpeed += 0.25;
		}
	}
	else{
		a = vec3(-enemyX,-enemyY,-enemyZ) - vec3(-transX, -transY, -transZ);
		if(sqrt(dot(a, a)) < 1.5){
			caught = true;
		}
	}
}

void checkWallCollisions(){

}

//Deletes all of the created objects here.
void CloseFunc(){
	window.window_handle = -1;
	cylinder.TakeDown();torus.TakeDown();square.TakeDown();square2.TakeDown();
	disc.TakeDown();sphere.TakeDown();sphere2.TakeDown();tiger.TakeDown();goldeen.TakeDown();
	stadium.TakeDown();stadiumRain.TakeDown();stadiumWinter.TakeDown();stadium2.TakeDown();coin.TakeDown();alien.TakeDown();
	shark.TakeDown();bird.TakeDown();sb.TakeDown();gengar.TakeDown();skyboxUW.TakeDown();fbo.TakeDown();rain2.TakeDown();
	tri2.TakeDown();skybox.TakeDown();skybox2.TakeDown();skybox3.TakeDown();egg.TakeDown();snow2.TakeDown();
	star.TakeDown();magneton.TakeDown();haunter.TakeDown();frog.TakeDown();
	sph1.TakeDown(); enm.TakeDown(); usr.TakeDown();sq4.TakeDown();
	delete coinX;delete coinZ;
}

//Maintains aspect ratio when window is resized.
void ReshapeFunc(int w, int h){
	if (h > 0){window.size = ivec2(w, h);window.window_aspect = float(w) / float(h);}
}

void KeyboardFunc(unsigned char c, int x, int y){
	float current_time = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

	switch (c){
	case 'b':  //Shifts backwards through menu
		if(menuOn){
			if(stadiumSelect > 0){
				if(singlePlayer || twoPlayer){
					if(gameSelect > 0){gameSelect = 0;}
					else{
						singlePlayer = false;
						twoPlayer = false;
					}
				}
				else{
					stadiumSelect = 0;
					menuSelect = 0;
				}
			}
		}
		//Goes back out of the weather menu
		if(weatherSelect){
			weatherSelect = false;
		}
		break;

	case 'n':  //Moves user down one unit
		transY = transY + 1;
		if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
			transY = transY - 1;
		}
		break;

	case 'u':  //Moves user up one unit
		transY = transY - 1;
		if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
			transY = transY + 1;
		}
		break;

	case 'i':  //Moves user forward one unit
		if(stadiumSelect == 1 || stadiumSelect == 2){
			transZ = (float)(transZ + 1*cos(-RotatedY*3.14/180));
			transX = (float)(transX + 1*sin(-RotatedY*3.14/180));
		}
		if(stadiumSelect == 3){
			transZ = (float)(transZ + 0.5*cos(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
			transX = (float)(transX + 0.5*sin(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
			transY = (float)(transY + 0.5*sin(RotatedX*3.14/180));
		}
		if(stadiumSelect == 2 && (sqrt(transX*transX + transZ*transZ)) > 52){
			transZ = (float)(transZ - 1*cos(-RotatedY*3.14/180));
			transX = (float)(transX - 1*sin(-RotatedY*3.14/180));
		}
		else if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
			transZ = (float)(transZ - 0.5*cos(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
			transX = (float)(transX - 0.5*sin(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
			transY = (float)(transY - 0.5*sin(RotatedX*3.14/180));
		}
		break;

	case 'k':  //Moves user backward one unit
		transZ = (float)(transZ - 1*cos(-RotatedY*3.14/180));
		transX = (float)(transX - 1*sin(-RotatedY*3.14/180));
		if(stadiumSelect == 2 && (sqrt(transX*transX + transZ*transZ)) > 52){
			transZ = (float)(transZ + 1*cos(-RotatedY*3.14/180));
			transX = (float)(transX + 1*sin(-RotatedY*3.14/180));
		}
		else if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
			transZ = (float)(transZ + 1*cos(-RotatedY*3.14/180));
			transX = (float)(transX + 1*sin(-RotatedY*3.14/180));
		}
		break;

	case 'j':  //Moves user left one unit
		transX = (float)(transX + 1*cos(RotatedY*3.14/180));
		transZ = (float)(transZ + 1*sin(RotatedY*3.14/180));
		if(stadiumSelect == 2 && (sqrt(transX*transX + transZ*transZ)) > 52){
			transX = (float)(transX - 1*cos(RotatedY*3.14/180));
			transZ = (float)(transZ - 1*sin(RotatedY*3.14/180));
		}
		else if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
			transX = (float)(transX - 1*cos(RotatedY*3.14/180));
			transZ = (float)(transZ - 1*sin(RotatedY*3.14/180));
		}
		break;

	case 'l':  //Moves user right one unit
		transX = (float)(transX - 1*cos(RotatedY*3.14/180));
		transZ = (float)(transZ - 1*sin(RotatedY*3.14/180));
		if(stadiumSelect == 2 && (sqrt(transX*transX + transZ*transZ)) > 52){
			transX = (float)(transX + 1*cos(RotatedY*3.14/180));
			transZ = (float)(transZ + 1*sin(RotatedY*3.14/180));
		}
		else if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
			transX = (float)(transX + 1*cos(RotatedY*3.14/180));
			transZ = (float)(transZ + 1*sin(RotatedY*3.14/180));
		}
		break;

	case 'p':
		if(!menuOn && !countdownOn){
			if (window.paused == true)
			{
				// Will be leaving paused state
				window.total_time_paused += (current_time - window.time_last_pause_began);
			}
			else
			{
				// Will be entering paused state
				window.time_last_pause_began = current_time;
			}
			window.paused = !window.paused;
		}
		break;

	case 'w':  //Toggles wireframe mode
		window.wireframe = !window.wireframe;
		break;

	case 'x':  //Exit the program
	case 27:
		glutLeaveMainLoop();
		return;
	case '1': 	
		//Select no weather and go back to the main menu
		if(weatherSelect){
			weatherSelect = false;weatherType = 0;
			if(stadiumSelect == 3){tod.y = 1;}else{tod.y=0;}
		}
		//Select 1st person perspective in certain single player modes
		else if(menuOn && menuSelect > 0 && singlePlayer && gameSelect>1){
			viewPerspective = 1;
			perspectiveOffset = 0.0f;
			perspectiveStepBack = 1.0f;
			countdownOn = true;
			menuOn = false;
			transY = -1;
			RotatedX = 0;
		}
		//Select Moshball for the game in either mode (always 1st person)
		else if(menuOn && menuSelect > 0 && (singlePlayer || twoPlayer)){
			gameSelect = 1;
			if(stadiumSelect == 1){
				for(int i=0; i<numSpheres; i++){hit[i] = 0;}
			}
			viewPerspective = 1;perspectiveOffset = 0.0f;perspectiveStepBack = 1.0f;
			countdownOn = true;menuOn = false;transY = -1;RotatedX = 0;
		}
		//Select single player
		else if(menuOn && menuSelect > 0){singlePlayer = true;}
		//Select jungle stadium
		else if(menuOn && menuSelect == 0){
			menuSelect = 1;stadiumSelect = 1;initCoins();initSpheres();initEnemy();
			if(tod.y != 2){tod.y = 0;}
		}
		return;
	case '2':  
		//Select fog and go back to the main menu
		if(weatherSelect){
			weatherType = 1;
			tod.y = 2;
			weatherSelect = false;			
		}
		//Select 3rd person view for single player games
		else if(menuOn && menuSelect > 0 && singlePlayer && gameSelect > 1){
			viewPerspective = 3;
			perspectiveOffset = 0.0f;
			perspectiveStepBack = 10.0f;
			countdownOn = true;
			menuOn = false;
			transY = 0;
		}
		//Select the avoid game for single player/ coin collection for 2 player
		else if(menuOn && menuSelect > 0 && (singlePlayer || twoPlayer)){
			gameSelect = 2;
		}
		//Select two player
		else if(menuOn && menuSelect > 0){
			twoPlayer = true;
		}
		//Select ufo stadium
		else if(menuOn && menuSelect == 0){
			menuSelect = 1;
			stadiumSelect = 2;
			initDiscCoins();
			initDiscSpheres();
			initEnemy();
			if(tod.y != 2){tod.y = 0;}
		}
		return;
	case '3':
		//Select rain and go back to the main menu
		if(weatherSelect){
			weatherType = 2;
			if(stadiumSelect == 3){tod.y = 1;}else{tod.y=0;}
			weatherSelect = false;			
		}
		//Select Coin Time Attack for single player mode
		else if(menuOn && menuSelect > 0 && singlePlayer){
			gameSelect = 3;
		}
		//Select underwater stadium
		else if(menuOn && menuSelect == 0){
			menuSelect = 1;
			stadiumSelect = 3;
			initSphereCoins();
			initSphereSpheres();
			initEnemy();
			if(tod.y != 2){tod.y = 1;}
		}
		return;
	case '4':
		//Select snow and go back to the main menu
		if(weatherSelect){
			weatherType = 3;
			if(stadiumSelect == 3){tod.y = 1;}else{tod.y=0;}
			weatherSelect = false;
			menuOn = true;			
		}
		//Select weather menu
		else if(menuOn && menuSelect == 0){
			weatherSelect = true;
		}
		else if(!menuOn){
			weatherType = (weatherType + 1)%4;
			if(weatherType == 1){tod.y = 2;}
			else{if(stadiumSelect == 3){tod.y = 1;}else{tod.y=0;}}
		}
		return;
	case '5':
		//Cycle through time of day
		//if(menuOn){
			tod.x  = (tod.x + 1)%3;
		//}
		return;
	case '6':
		vsPokemon = !vsPokemon;
		return;
	case 32:   //space bar
		return;
	}	
}

void SpecialFunc(int c, int x, int y)
{
	switch(c){
        case GLUT_KEY_UP:rotateX(1);return;
        case GLUT_KEY_DOWN:rotateX(-1);return;
        case GLUT_KEY_RIGHT:rotateY(-1);return;
        case GLUT_KEY_LEFT:rotateY(1);return;
		case GLUT_KEY_F1:	
			weatherType = (weatherType + 1)%4;
			if(weatherType == 1){tod.y = 2;}
			else{if(stadiumSelect == 3){tod.y = 1;}else{tod.y=0;}}
			return;
		case GLUT_KEY_F2:
			tod.x  = (tod.x + 1)%3;
			return;
		case GLUT_KEY_F3:
			vsPokemon = !vsPokemon;
			return;
    }
}

//Orchestrates all the objects and variables into a playable game
void GameDisplay(){
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.35f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection = perspective(25.0f, window.window_aspect, 0.01f, 600.0f);
	modelview = lookAt(vec3(0.0f, perspectiveOffset, perspectiveStepBack), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	modelview = render(modelview);
	glLoadMatrixf(value_ptr(modelview));
	glPolygonMode(GL_FRONT_AND_BACK, window.wireframe ? GL_LINE : GL_FILL);	
	/////////////////////////////Menu Display////////////////////////////////
	if(menuOn){
		//Main Menu
		if(menuFirst == 1){
			init((ALbyte *)"./music/menuMusic.wav", true);
			menuFirst = 4;
		}
		menuTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
		if(menuSelect == 0){
			if(!weatherSelect){textDisplay(0, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
			else{textDisplay(1, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
		}
		//Stadium Selection
		else if(menuSelect == 1){
			if(!(singlePlayer || twoPlayer)){textDisplay(2, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
			else if(singlePlayer){
				if(gameSelect == 0){					
					if(stadiumSelect == 1){textDisplay(3, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
					else if(stadiumSelect == 2){textDisplay(4, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
					else if(stadiumSelect == 3){textDisplay(5, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
				}
				else{textDisplay(6, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}				
			}
			else if(twoPlayer){textDisplay(7, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
		}
	}
	//Countdown Display
	else if(countdownOn){
			if(first == 1){
				alSourceStop(origSource);
				if(stadiumSelect == 1){init((ALbyte *)"./music/havingAWildTime.wav", true);}
				else if(stadiumSelect == 2){init((ALbyte *)"./music/gummi3.wav", true);}
				else{init((ALbyte *)"./music/adventureInAtlantica.wav", true);}
				first = 2;
			}

			countdownTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;countdownTime -= menuTime;
			if(countdownTime < 4 && countdownTime >= 0){textDisplay(8, (float)(window.size.x), (float)(window.size.y), tod.x, vsPokemon);}
			if(countdownTime >= 4){countdownOn = false;}
	}

	///////////////////////Game Display///////////////////////////
	else{
		current_timeDisplay = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
		current_timeDisplay = (window.paused ? window.time_last_pause_began : current_timeDisplay) - window.total_time_paused;
		
		//Mouse movement
		mouseRotations(stadiumSelect, viewPerspective);
		//Non-linear wall collisions
		if(!caught){
			if(stadiumSelect == 1 || stadiumSelect == 2){
				transZ = (float)(transZ + velocity*cos(-RotatedY*3.14/180));
				transX = (float)(transX + velocity*sin(-RotatedY*3.14/180));
			}
			if(stadiumSelect == 3){
				transZ = (float)(transZ + 0.5*cos(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
				transX = (float)(transX + 0.5*sin(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
				transY = (float)(transY + 0.5*sin(RotatedX*3.14/180));
			}
			if(stadiumSelect == 2 && (sqrt(transX*transX + transZ*transZ)) > 52){
				if(gameSelect != 1){caught = true;}
				else{transZ = 0.0f;transX = 0.0f;}
			}
			else if(stadiumSelect == 3 && (sqrt(dot(vec3(transX, transY, transZ), vec3(transX, transY, transZ)))) > 51.8){
				transZ = (float)(transZ - 0.5*cos(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
				transX = (float)(transX - 0.5*sin(-RotatedY*3.14/180)*cos(RotatedX*3.14/180));
				transY = (float)(transY - 0.5*sin(RotatedX*3.14/180));
				RotatedY -= 180.0f;
			}
		}
		//Music change when caught
		if(caught){
			if(first == 2){
				alSourceStop(origSource);
				if(stadiumSelect == 1){
					if(vsPokemon){init((ALbyte *)"./music/gengar.wav", false);}
					else{init((ALbyte *)"./music/tiger.wav", false);}
					first = 3;
				}
				else if(stadiumSelect == 2){
					if(vsPokemon){init((ALbyte *)"./music/magneton.wav", false);}
					else{init((ALbyte *)"./music/alien.wav", false);}
					first = 3;
				}
				else{
					if(vsPokemon){init((ALbyte *)"./music/tentacool.wav", false);}
					else{init((ALbyte *)"./music/sharkLaugh.wav", false);}					
					first = 3;
				}
			}
			if(first == 3){
				if(stadiumSelect == 1){init((ALbyte *)"./music/deepJungle.wav", true);}
				else if(stadiumSelect == 2){init((ALbyte *)"./music/whatIsLove.wav", true);}
				else{init((ALbyte *)"./music/aVerySmallWish.wav", true);}
				first = 4;
			}
		}
		
		//Determine whether or not to generate stars as coins
		modelview = translate(modelview, vec3(0,1,0));
		if(!(gameSelect == 1)){
			makeCoins(modelview, projection);
			float x, r, z;
			mat4 another;
			if(stadiumSelect != 3){
				for(int i=0; i < numCoins; i++){
					if(coinX[i] == -1000){
						if(stadiumSelect == 1){
							x = (float)(rand() % 1006 - 518);x = x/10;
							z = (float)(rand() % 1006 - 518);z = z/10;

							coinX[i] = x;
							coinZ[i] = z;
						}
						else if(stadiumSelect == 2){
							r = (float)(rand() % 518);
							r = r/10;
							x = (float)(rand() % 628);
							x = x/100;

							coinX[i] = r * cos(x);
							coinZ[i] = r * sin(x);
						}
					}
					another = translate(modelview, vec3(coinX[i],-0.75,coinZ[i]));
					another = rotate(another, 500* current_timeDisplay + 90.0f, vec3(0,1,0));
					mat4 scaler;
					scaler = another;
					scaler = scale(scaler, vec3(0.1, 0.1, 0.1));
					star.Draw(projection, scaler, tod, 0);
				}
			}
			//Make the enemy
			makeEnemy(modelview, projection);
		}
		//Make the spheres
		makeSpheres(modelview, projection);
		//Take into account music loading delay
		if(startupTime){
			countdownTime = current_timeDisplay - menuTime;
			startupTime = false;
		}
		//Jungle Stadium
		if(stadiumSelect == 1){
			//Make the skybox based on weather//
			modelview = endRender(modelview);

			modelview = rotate(modelview, (GLfloat) RotatedX, vec3(1,0,0));
			modelview = rotate(modelview, (GLfloat) RotatedY, vec3(0,1,0));
			if(weatherType == 0){skybox.Draw(projection, modelview, tod, 0);}
			else if(weatherType == 1 || weatherType == 2){skybox3.Draw(projection, modelview, tod, 0);}
			else if(weatherType == 3){skybox2.Draw(projection, modelview, tod, 0);}
			//Make precipitation
			if(weatherType > 1){
				mat4 another;
				for(int i=0; i<numParts; i++){
					another = rotate(modelview, 90.0f, vec3(1,0,0));
					another = translate(another, vec3(partX[i],partY[i],3*tan((current_timeDisplay)+partOffset[i])));
					another = rotate(another, 270.0f, vec3(1,0,0));
					another = rotate(another, 90.0f, vec3(0,1,0));
					if(weatherType == 2){rain2.Draw(projection, another, tod, 0);}
					else{snow2.Draw(projection, another, tod, 0);}
				}
			}
			
			modelview = rotate(modelview, (GLfloat) -RotatedY, vec3(0,1,0));
			modelview = rotate(modelview, (GLfloat) -RotatedX, vec3(1,0,0));

			//Set up orthographic projection for radar
			mat4 projection1 = perspective(45.0f, window.window_aspect, 0.01f, 10.0f);
			glm::mat4 Projection2 = glm::ortho( -10.0f, 10.0f, -10.0f, 00.0f,1.0f, 10.0f);
			glm::mat4 View       = glm::lookAt(
				glm::vec3(0,0,5), // Camera is at (0,0,5), in World Space
				glm::vec3(0,0,0), // and looks at the origin
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);  
			
			mat4 another = View;
			another = translate(another, vec3(-6,-7.6,0));
			mat4 radar = another;
			another = rotate(another, 90.0f, vec3(0,1,0));
			another = rotate(another, 90.0f, vec3(0,0,1));

			//Radar display
			square.Draw(Projection2, another, tod, 0);

			for(int i=0; i < numSpheres; i++){
				another = translate(View, vec3(-7.5 + ((3/104.6)*sphereX[i]),-8.1 - ((3/104.6)*sphereZ[i]),0));
				mat4 scaler = scale(another, vec3(0.05f, 0.05f, 0.05f));
				if(hit[i] == 0){sphere2.Draw(Projection2, scaler, tod, 0);}
				else{sphere.Draw(Projection2, scaler, tod, 0);}
			}

			if(gameSelect != 1){
			for(int i=0; i < numCoins; i++){
				another = translate(View, vec3(-7.5 + ((3/104.6)*coinX[i]),-8.1 - ((3/104.6)*coinZ[i]),0));
				sph1.Draw(Projection2, another, tod, 0);				
			}
			if(gameSelect != 3){
				another = translate(View, vec3(-7.5 - ((3/104.6)*enemyX),-8.1 + ((3/104.6)*enemyZ),0));
				enm.Draw(Projection2, another, tod, 0);
			}
			}

			another = translate(View, vec3(-7.5 - ((3/104.6)*transX),-8.1 + ((3/104.6)*transZ),0));
			usr.Draw(Projection2, another, tod, 0);

			modelview = render(modelview);
			
			//Used for displaying floats as strings
			stringstream tt (stringstream::in | stringstream::out);

			//Stop the clock, user, and score calculations when caught
			if(!caught){
				tt << current_timeDisplay-menuTime-countdownTime;
				
				timeString = tt.str();

				modelview = endRender(modelview);
				
				checkBallCollisions();
				if(gameSelect != 1){checkCoinCollisions();if(gameSelect != 3){checkEnemyCollision();}}
				if(viewPerspective == 3){
					mat4 scaler;
					scaler = translate(modelview, vec3(0.0f, -0.75f, 0.0f));
					scaler = rotate(scaler, 180.0f, vec3(0,1,0));
					scaler = scale(scaler, vec3(0.1f, 0.1f, 0.1f));
					frog.Draw(projection, scaler, window.size, 0);} //Draw Mew in 3rd perspective
			
				modelview = render(modelview);
			}			
			else{
				if(gameSelect == 3){ //End time on 100 for time attack
					if(current_timeDisplay - menuTime - countdownTime >= 100){
						current_timeDisplay = 100 + menuTime + countdownTime;
					}
					tt << current_timeDisplay-menuTime-countdownTime;
					timeString = tt.str();
				}
				//Maintain values for 2 player game
				if(twoPlayer){
					firstPersonTime = current_timeDisplay;firstPersonScore = (float)(score);
					initCoins();initSpheres();initEnemy();
					secondPlayerTurn = true;caught = false;
				}
			}
			//Change the stadium to match the weather
			modelview = translate(modelview, vec3(52.8,-1,52.8));
			if(weatherType == 0 || weatherType == 1){stadium.Draw(projection, modelview, tod, 0.0f);}
			else if(weatherType == 2){stadiumRain.Draw(projection, modelview, tod, 0.0f);}
			else{stadiumWinter.Draw(projection, modelview, tod, 0.0f);}
			modelview = translate(modelview, vec3(-52.8,0,-52.8));
		}
		//Disc Stadium
		else if(stadiumSelect == 2){
			modelview = translate(modelview, vec3(0,-1,0));
			stadium1.Draw(projection, modelview, tod, 0);
			modelview = translate(modelview, vec3(0,1,0));

			modelview = endRender(modelview);
			//Precipitation inside the UFO
			modelview = rotate(modelview, (GLfloat) RotatedX, vec3(1,0,0));
			modelview = rotate(modelview, (GLfloat) RotatedY, vec3(0,1,0));
			if(weatherType > 1){
				mat4 another;
				for(int i=0; i<numParts; i++){
					another = rotate(modelview, 90.0f, vec3(1,0,0));
					another = translate(another, vec3(partX[i],partY[i],3*tan((current_timeDisplay)+partOffset[i])));
					another = rotate(another, 270.0f, vec3(1,0,0));
					another = rotate(another, 90.0f, vec3(0,1,0));
					if(weatherType == 2){rain2.Draw(projection, another, tod, 0);}
					else{snow2.Draw(projection, another, tod, 0);}
				}
			}
			
			modelview = rotate(modelview, (GLfloat) -RotatedY, vec3(0,1,0));
			modelview = rotate(modelview, (GLfloat) -RotatedX, vec3(1,0,0));

			modelview = render(modelview);
			//Game mode management
			stringstream tt (stringstream::in | stringstream::out);
			if(!caught){
				tt << current_timeDisplay-menuTime-countdownTime;
				timeString = tt.str();

				modelview = endRender(modelview);
				
				checkBallCollisions();
				if(gameSelect != 1){checkCoinCollisions();checkEnemyCollision();}
				if(viewPerspective == 3){mat4 scaler;
					scaler = translate(modelview, vec3(0.0f, -0.75f, 0.0f));
					scaler = rotate(scaler, 180.0f, vec3(0,1,0));
					scaler = scale(scaler, vec3(0.1f, 0.1f, 0.1f));
					frog.Draw(projection, scaler, window.size, 0);}
			
				modelview = render(modelview);
			}
		}
		//Sphere Stadium
		else if(stadiumSelect == 3){

			modelview = endRender(modelview);
			modelview = rotate(modelview, (GLfloat) RotatedX, vec3(1,0,0));
			modelview = rotate(modelview, (GLfloat) RotatedY, vec3(0,1,0));
			//Draw underwater skybox and precipitation
			skyboxUW.Draw(projection, modelview, tod, 0);
			if(weatherType > 1){
				mat4 another;
				for(int i=0; i<numParts; i++){
					another = rotate(modelview, 90.0f, vec3(1,0,0));
					another = translate(another, vec3(partX[i],partY[i],3*tan((current_timeDisplay)+partOffset[i])));
					another = rotate(another, 270.0f, vec3(1,0,0));another = rotate(another, 90.0f, vec3(0,1,0));
					if(weatherType == 2){rain2.Draw(projection, another, window.size, 0);}
					else{snow2.Draw(projection, another, window.size, 0);}
				}
			}
			modelview = rotate(modelview, (GLfloat) -RotatedY, vec3(0,1,0));
			modelview = rotate(modelview, (GLfloat) -RotatedX, vec3(1,0,0));
			modelview = render(modelview);
			//Game mode management
			stadium2.Draw(projection, modelview, tod, 0);
			stringstream tt (stringstream::in | stringstream::out);
			if(!caught){
				tt << current_timeDisplay-menuTime-countdownTime;
				timeString = tt.str();

				modelview = endRender(modelview);
				
				checkBallCollisions();checkCoinCollisions();checkEnemyCollision();
				if(viewPerspective == 3){mat4 scaler;
					scaler = translate(modelview, vec3(0.0f, -0.75f, 0.0f));
					scaler = rotate(scaler, 180.0f, vec3(0,1,0));
					scaler = scale(scaler, vec3(0.1f, 0.1f, 0.1f));
					frog.Draw(projection, scaler, window.size, 0);}
				modelview = render(modelview);
			}
		}
		//Display Time, crosshairs, and Targets/Score
		if(viewPerspective == 1 && !caught){activeTextDisplay("+", -0.015f, -0.06f);}
		activeTextDisplay(("Time: " + timeString).c_str(), 0.5f, -0.9f);
		stringstream ss (stringstream::in | stringstream::out);
		if(gameSelect == 1){ss << targets;}
		else{ss << score;}
		string scoreString = ss.str();
		if(gameSelect == 1){activeTextDisplay(("Targets: " + scoreString).c_str(), 0.0f, -0.9f);}
		else{activeTextDisplay(("Score: " + scoreString).c_str(), 0.0f, -0.9f);}
		if(gameSelect == 1 && targets == 0){
			activeTextDisplay("You win!", -0.1f, 0.0f);
			caught = true;
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFlush();
	
}

//Put objects into the fbo
void RenderIntoFrameBuffer(){fbo.Bind();GameDisplay();fbo.Unbind();}

//Give fbo texture id to an object
void UseFramebufferToDrawSomething(){	
	glBindTexture(GL_TEXTURE_2D, fbo.texture_handles[0]);
	glEnable(GL_TEXTURE_2D);
	stadium.jb->fboID = fbo.texture_handles[0];
	stadiumRain.jb->fboID = fbo.texture_handles[0];
	stadiumWinter.jb->fboID = fbo.texture_handles[0];
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

//Manage pausing and when to display the game / jumbotrons
void DisplayFunc(){
	glViewport(0, 0, window.size.x, window.size.y);
	if(!window.paused){
	GameDisplay();	
	if(stadiumSelect == 1){
		glViewport(0, 0, (GLsizei)(512.0), (GLsizei)(512.0));
		RenderIntoFrameBuffer();
		UseFramebufferToDrawSomething();
	}	
	glutSwapBuffers();
	}
	
}

void TimerFunc(int value){
	if (window.window_handle != -1){glutTimerFunc(window.interval, TimerFunc, value);glutPostRedisplay();}
}

int main(int argc, char * argv[])
{
	glewInit();
	glutInit(&argc, argv);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	//Initialize object's colors / values
	torus.color = vec3(1.0f, 0.1f, 0.0f);
	square.color1 = vec3(1.0, 1.0, 1.0);
	square2.color1 = vec3(0.95f, 0.1f, 0.7f);
	square2.color2 = vec3(1.0f, 1.0f, 0.0f);
	disc.color = vec3(1.0f, 0.25f, 0.0f);
	sphere.color = vec3(0.15f, 0.45f, 0.0f);
	sphere2.color = vec3(0.45f, 0.05f, 0.25f);
	cylinder.color = vec3(0.0f, 0.5f, 0.0f);
	gengar.texID = 18;
	egg.texID = 20;
	star.texID = 22;
	magneton.texID = 25;
	haunter.texID = 26;
	frog.texID = 27;
	goldeen.texID = 76;
	rain2.color = vec3(0.4f, 0.4f, 1.0f);
	snow2.color = vec3(1.0f, 1.0f, 1.0f);
	fontfilename = "Pokemon.ttf";

	//Initialize the precipitation
	numParts = 250;
	partX = new float[numParts];
	partY = new float[numParts];
	partOffset = new float[numParts];
	for(int i=0; i<numParts; i++){
		partX[i] = (float)(rand() % 1200);
		partX[i] = (partX[i] - 600)/100;

		partY[i] = (float)(rand() % 1200);
		partY[i] = (partY[i] - 600)/100;
		partOffset[i] = (float)(rand() % 360 + 1);
	}
	sphere.lava = true;sphere2.lava = false; //Initialize sphere statuses

	window.window_handle = glutCreateWindow("Moshball");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	
	glutMotionFunc(mouseMovement);
	glutPassiveMotionFunc(mouseMovement); //check for mouse movement
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	
	//Initialize everything

	if (glewInit() != GLEW_OK)
	{
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	init_resources();
	glutDisplayFunc(DisplayFunc);

	if (!fbo.Initialize(glm::ivec2(512, 512), 1, true)){
		cerr << "Frame buffer failed to initialize." << endl;
		return 0;
	}

	sph1.color = vec3(1.0f, 0.85f, 0.0f);
	if (!sph1.Initialize(2,0.05f, "phong.vert", "phong.frag")){return 0;}
	enm.color = vec3(0.75f, 0.0f, 0.75f);
	if (!enm.Initialize(2,0.05f, "phong.vert", "phong.frag")){return 0;}
	usr.color = vec3(0.0f, 0.0f, 0.5f);
	if (!usr.Initialize(2,0.05f, "phong.vert", "phong.frag")){return 0;}
	if (!sb.Initialize()){return 0;}
	if (!coin.Initialize()){return 0;}
	if (!stadium.Initialize(0)){return 0;}
	if (!stadiumRain.Initialize(1)){return 0;}
	if (!stadiumWinter.Initialize(2)){return 0;}
	if (!stadium1.Initialize()){return 0;}
	if (!stadium2.Initialize(31, 52.8f)){return 0;}
	if (!skybox.Initialize(0, 5000, "basic_skybox_shader.vert", "basic_skybox_shader.frag")){return 0;}
	if (!skybox2.Initialize(1, 5025, "basic_skybox_shader.vert", "basic_skybox_shader.frag")){return 0;}
	if (!skybox3.Initialize(2, 5050, "basic_skybox_shader.vert", "basic_skybox_shader.frag")){return 0;}
	if (!skyboxUW.Initialize(3, 5075, "basic_skybox_shader.vert", "basic_skybox_shader.frag")){return 0;}
	if (!tiger.Initialize()){return 0;}
	if (!alien.Initialize()){return 0;}
	if (!shark.Initialize()){return 0;}

	if (!star.Initialize("./models/star.obj", "./models/star.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag")){return 0;}	
	if (!gengar.Initialize("./models/gengar.obj", "./models/gengar.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag")){return 0;}
	if (!magneton.Initialize("./models/magneton.obj", "./models/magneton.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag")){return 0;}
	if (!haunter.Initialize("./models/tentacool.obj", "./models/tentacool.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag")){return 0;}
	if (!frog.Initialize("./models/Mew.obj", "./models/Mew.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag")){return 0;}

	if (!sphere.Initialize(15, 1, "sphereShader2.vert", "sphereShader2.frag")){return 0;}
	if (!sphere2.Initialize(15, 1, "sphereShader2.vert", "sphereShader2.frag")){return 0;}
	if (!rain2.Initialize(3, 0.03f, "phong.vert", "phong.frag")){return 0;}
	if (!snow2.Initialize(3, 0.02f, "phong.vert", "phong.frag")){return 0;}
	if (!egg.Initialize(15, 1, "./textures/alienEgg.jpg", "basic_texture_shader.vert", "basic_texture_shader.frag")){return 0;}
	if (!square.Initialize(1, 3.0f, "phong.vert", "phong.frag")) {return 0;}
	if (!sq4.Initialize(1, 5.0f, "basic_texture_shader.vert", "basic_texture_shader.frag")) {return 0;}

	glutMainLoop();
}