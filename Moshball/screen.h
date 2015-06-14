#pragma once
#include "object2.h"
#include "shader2.h"
#include "texturedshader.h"
#include "TextureManager.h"

/*	This is a concrete instanciation of the abstract Object() class.
*/

class Screen : public Object2
{
public:
	Screen();
	virtual bool Initialize(float x, float y);
	virtual void Draw(const glm::mat4& projection, glm::mat4 modelview, const glm::ivec2 & size, const float time = 0);
	void TakeDown();
	void StepShader();
	TextureManager * tm;
	unsigned int texID;

private:
	typedef Object2 super;
	int shader_index;

	//	BAD DESIGN WARNING...
	//	To improve O.O. design, the triangle should NOT be aware of what shader or texture is
	//	being used  to render  it.  Therefore, none of the lines below should really be here.

	Shader2 pattern_shader;
	Shader2 basic_shader;
	Shader2 colored_shader;
	Shader2 play_shader;
	TexturedShader basic_texture_shader;
	std::vector<Shader2 *> shaders;
};
