#pragma once

#include "FBO.hpp"
#include "Shader.hpp"
#include "shapes/Rectangle2D.hpp"
#include "texture/Texture2D.hpp"

struct RenderTexture2D {
  RenderTexture2D() = default;

  RenderTexture2D(const TextureDescriptor& desc)
    : texture(desc),
      fbo(FBO(1)) {
    fbo.attach2D(GL_COLOR_ATTACHMENT0, texture);
  }

  void operator=(RenderTexture2D rhs) {
    if (fbo.id)
      fbo.dettach2D(GL_COLOR_ATTACHMENT0);

    texture = rhs.texture;
    fbo = rhs.fbo;
  }

  void clear(vec4 color = vec4(vec3(0.f), 1.f)) {
    fbo.bind();
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    fbo.unbind();
  }

  void draw(const Rectangle2D& area, const Shader& shader) {
    fbo.bind();
    area.draw(shader);
  }

  Texture2D texture;
  FBO fbo;
};

