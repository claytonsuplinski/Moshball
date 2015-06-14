// Font class header.
#ifndef FONT_HPP
#define FONT_HPP
 
// Storing {char:glyph} pairs.
#include <map>
 
// Include files for FreeType 2.
#include "ft2build.h"
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
 
// The various type structures we created earlier.
#include "types.h"
 
// Dirty macro to determine the offset of a field within a struct.
// Used to determine the offsets of data in the GPU buffers.
#define VBO_OFFSET(count, vertex, field) \
(void*)((count * sizeof(vertex)) + (long int)&(((vertex*)NULL)->field))
 
class CFont
{
public:
CFont();
~CFont();
 
/**
* Initializes the FreeType library.
* This should be called prior to ANY other font operations.
* Since it is static, no instance of CFont needs to be present,
* simply do:
* if(!CFont::Initialize())
* std::cerr << "FreeType 2 failed to initialize!\n";
*
* @return TRUE on success, FALSE on failure.
**/
static bool Initialize();
 
/**
* Loads a TrueType font.
* Here, all of the printable characters in the ASCII table
* are loaded as bitmaps, then are turned into renderable
* textures in GPU memory. Their dimensions are stored for
* rendering later on.
*
* @param std::string Font filename
* @paarm uint16_t Font size (optional=12)
*
* @pre CFont::Initialize() must have been called.
*
* @return TRUE if everything went smoothly,
* FALSE otherwise.
**/
bool LoadFromFile(const std::string& filename,
const uint16_t size = 12);
 
/**
* Resizes the font.
* This stores a new font size and calls LoadFromFile()
* again, regenerating new bitmaps and textures for all
* characters.
* Since this can be a relatively expensive operation,
* it should be used sparingly. If you think you're going
* to be resizing a font often, consider caching multiple
* instances of CFont.
*
* @param uint16_t New font size
*
* @return Value of CFont::LoadFromFile()
**/
bool Resize(const uint32_t size);
 
/**
* Renders text on the current framebuffer.
* Given a string, this function will render it at
* the given position. A VBO will be created, vertex
* information generated, and then each character will
* be rendered using its texture.
* The position parameter marks the bottom of the "line"
* to be drawn on, so parts of characters such as 'j'
* or 'q' will fall below the line. Think of a sheet of
* paper.
* A rectangle is returned, in case it is useful to know
* the overall dimensions of the text rendered.
*
* @param std::string Text to render
* @param math::vector2_t Position to start rendering
*
* @return Rectangle representing rendered text dimensions.
*
* @see GetTextWidth()
* @see GetTextHeight()
**/
math::rect_t RenderText(const std::string& text,
const math::vector2_t& Pos);
 
/**
* Sets text color; the default is white.
**/
void SetFontColor(const float r, const float g, const float b);
 
private:
static FT_Library s_Library;
static bool s_loaded;
 
// Glyph representing data for each character.
struct Glyph
{
// OpenGL texture handle for the bitmap
uint32_t texture;
// Dimensions and offsets.
rect_t dim;
};
 
// {Char: Glyph} dictionary.
std::map<char, Glyph> mp_glyphTextures;
 
FT_Face m_FontFace;
 
color4f_t m_Color;
std::string m_filename;
uint16_t m_size;
bool m_loaded, m_ready;
};
 
#endif // FONT_HPP