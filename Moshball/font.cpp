/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 2

Creates a stool object out of cylinders, discs, a torus, and stool legs.
*/

#include <iostream>
#include "font.h"

using namespace std;

bool CFont::Initialize()
{
if(s_loaded) return true;
if(FT_Init_FreeType(&s_Library) != 0) return false;
return (s_loaded = true);
}


CFont::CFont() : m_ready(false), m_loaded(false)
{
// Initialize FreeType.
if(!CFont::Initialize()) return;
// Load the shader.
if(!m_FontRender.LoadFromFile("Default.vs", "FontRender.fs"))
return;
// Give the font shader an identity matrix for the model-view
// and the projection matrix that was created with the window.
m_FontRender.Bind();
if(!m_FontRender.SetMatrix("modelview", math::IDENTITY_MATRIX) ||
!m_FontRender.SetMatrix("proj", CWindow::GetProjectionMatrix()))
return;
m_FontRender.Unbind();
m_ready = true;
}

bool CFont::LoadFromFile(const std::string& filename, const uint16_t size)
{
// Make sure everything is ready.
if(!m_ready || !CFont::s_loaded) return false;
// Create a new font face.
if(FT_New_Face(s_Library, filename.c_str(), 0, &m_FontFace) != 0)
return false;
 
// Set the size of the font face.
// Since the function expects a size in 1/64 pixels, we multiply
// by 64 (same as left-shifting 6 bits) before passing.
// The 96 represents a 96-dpi font bitmap.
if(FT_Set_Char_Size(m_FontFace, size << 6, size << 6, 96, 96) != 0)
return false;
 
// Load all printable characters.
// If you visit an ASCII table (like www.asciitable.com) you will see
// that the only valid values for printing are the space character all
// the way up to the tilde (~).
for(size_t i = ' '; i <= '~'; ++i)
{
FT_Glyph glyph;
 
// Locate the index of the character in the font face.
uint32_t index = FT_Get_Char_Index(m_FontFace, i);
if(index == 0) continue;
 
// Load the glyph into the font face.
FT_Load_Glyph(m_FontFace, index, FT_LOAD_RENDER);
 
// Render the glyph as a mono-chrome bitmap.
FT_Render_Glyph(m_FontFace->glyph, FT_RENDER_MODE_NORMAL);
 
// Put the glyph in the glyph slot into an actual glpyh struct.
FT_Get_Glyph(m_FontFace->glyph, &glyph);
 
// Small shortcuts
FT_GlyphSlot slot = m_FontFace->glyph;
FT_Bitmap& bitmap = slot->bitmap;
 
// Bitmap dimensions
uint32_t w = bitmap.width;
uint32_t h = bitmap.rows;
 
uint32_t texture = 0;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
 
// We need to copy the data over to a new buffer in order
// to properly pass it to the GPU.
unsigned char* data = new unsigned char[w * h];
memset(data, NULL, w * h * sizeof(unsigned char));
memcpy(data, bitmap.buffer, sizeof(unsigned char) * w * h);
 
// Alignment for pixels needs to be at 1 byte.
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 0, GL_RED, w, h,
GL_UNSIGNED_BYTE, data);
// Restore default alignment value. I haven't actually tested this
// part so it may or may not actually be the default.
glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
// Delete bitmap buffers
delete[] data;
FT_Done_Glyph(glyph);
 
// Our custom glyph structure.
CFont::Glyph render_glyph;
render_glyph.pTexture = pTexture;
 
// The dimensions are organized as follows:
// x: total bitmap width
// y: total bitmap height
// w: advancement to the next character
// h: height offset from the top of the character to the line
// w and h are represented in 1/64ths of a pixel so we need
// to convert them to accurate on-screen pixels.
render_glyph.dim = math::rect_t(w, h,
slot->advance.x >> 6,
slot->metrics.horiBearingY >> 6);
// Assign the glyph to its slot in the dictionary.
mp_glyphTextures[i] = render_glyph;
}
 
// Clean up the font face.
FT_Done_Face(m_FontFace);
 
// Store the filename internally in-case we need to Resize() later.
m_filename = filename;
// We're done!
return (m_loaded = true);
}
