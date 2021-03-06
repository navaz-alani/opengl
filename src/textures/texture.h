#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>

#include <GL/glew.h>

enum class TextureType : unsigned int {
  _1D = GL_TEXTURE_1D,
  _2D = GL_TEXTURE_2D,
  _3D = GL_TEXTURE_3D,
};

inline unsigned int GLTextureType(const TextureType t) {
  return static_cast<unsigned int>(t);
}

class Texture {
  unsigned int   m_glID;
  TextureType    m_type;
  std::string    m_file;
  unsigned char *m_buffer;
  int m_height,
      m_width,
      m_bpp; // bits per pixel

  void setTexture();
public:
  Texture(const std::string &filepath, TextureType t = TextureType::_2D);
  ~Texture();

  void Bind(const unsigned int slot = 0) const;
  void Unbind() const;

  inline int width() const { return m_width; }
  inline int height() const { return m_height; }
};

#endif
