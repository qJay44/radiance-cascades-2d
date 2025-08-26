#pragma once

struct TextureDescriptor {
  GLenum internalFormat = GL_RGB8;          // Color format in the OpenGL
  GLenum format         = GL_RGB;           // Color format of the loaded image
  GLenum type           = GL_UNSIGNED_BYTE; // Color bytes format of the loaded image
  GLenum minFilter      = GL_LINEAR;
  GLenum magFilter      = GL_LINEAR;
  GLenum wrapS          = GL_REPEAT;
  GLenum wrapT          = GL_REPEAT;
  GLenum wrapR          = GL_REPEAT;
};

