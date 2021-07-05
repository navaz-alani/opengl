#include "texture.h"
#include "../logger/logger.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string &filepath, TextureType t)
  : m_glID{ 0 },
    m_type{ t },
    m_buffer{ nullptr},
    m_isInitialised{ true },
    m_height{ 0 },
    m_width{ 0 },
    m_bpp{ 0 } {
  // create texture
  glGenTextures(1, &m_glID);
  glBindTexture(GLTextureType(t), m_glID);

  // load texture
  Logger log;
  // this flag ensures that the image is flipped since OpenGL requires the
  // texture pixels to start at the bottom left instead of top left;
  stbi_set_flip_vertically_on_load(1);
  m_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 4); // 4 channels -> RGBA
  if (!m_buffer) {
    log << LoggerState::Error
        << "texture file "
        << filepath
        << " does not exist\n";
    exit(1);
  }

  // setup some sensible texture parameters
  glTexParameteri(GLTextureType(t), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GLTextureType(t), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GLTextureType(t), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // wrap x
  glTexParameteri(GLTextureType(t), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // wrap y

  // send texture to OpenGL
  setTexture();
  // delete local buffer when done
  if (m_buffer) {
    stbi_image_free(m_buffer);
    m_buffer = nullptr;
  }
}

Texture::Texture(TextureType t)
  : m_glID{ 0 },
    m_type{ t },
    m_buffer{ nullptr},
    m_isInitialised{ false },
    m_height{ 0 },
    m_width{ 0 },
    m_bpp{ 0 } {
  // create texture - but don't bind
  glGenTextures(1, &m_glID);
  // setup some sensible texture parameters
  glTexParameteri(GLTextureType(t), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GLTextureType(t), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GLTextureType(t), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // wrap x
  glTexParameteri(GLTextureType(t), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // wrap y
}

void Texture::initialize(const std::string &filepath) {
  glBindTexture(GLTextureType(m_type), m_glID);
  // load texture
  Logger log;
  // this flag ensures that the image is flipped since OpenGL requires the
  // texture pixels to start at the bottom left instead of top left;
  stbi_set_flip_vertically_on_load(1);
  m_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_bpp, 4); // 4 channels -> RGBA
  if (!m_buffer) {
    log << LoggerState::Error
        << "texture file "
        << filepath
        << " does not exist\n";
    exit(1);
  }
  m_isInitialised = true;
}

void Texture::setTexture() {
  // we only work with 2D textures for now...
  switch (m_type) {
    case TextureType::_1D: { assert(false); }
    case TextureType::_2D: {
      glTexImage2D(
        GLTextureType(m_type), 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer
      );
      break;
    }
    case TextureType::_3D: { assert(false); }
  }
}

Texture::~Texture() {
  glDeleteTextures(1, &m_glID);
}

void Texture::Bind(const unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GLTextureType(m_type), m_glID);
}

void Texture::Unbind() const {
  glBindTexture(GLTextureType(m_type), 0);
}
