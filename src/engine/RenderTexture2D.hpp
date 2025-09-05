#pragma once

#include "FBO.hpp"
#include "Shader.hpp"
#include "shapes/Rectangle2D.hpp"
#include "texture/Texture2D.hpp"
#include "texture/TextureDescriptor.hpp"

struct RenderTexture2D {
  RenderTexture2D() {}

  RenderTexture2D(const TextureDescriptor& desc)
    : texture(desc),
      fbo(FBO(1)) {
    fbo.attach2D(GL_COLOR_ATTACHMENT0, texture);
  }

  void operator=(RenderTexture2D rhs) {
    texture = rhs.texture;
    fbo = rhs.fbo;
  }

  void clear(vec4 color = vec4(0.f)) {
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void draw(const Rectangle2D& area, const Shader& shader, const Texture2D* bindTex = nullptr) {
    fbo.bind();

    if (bindTex) bindTex->bind();
    area.draw(shader);
    Texture2D::unbind(GL_TEXTURE_2D);
  }

  Texture2D texture;
  FBO fbo;
};

