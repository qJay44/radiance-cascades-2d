#pragma once

#include "FBO.hpp"
#include "Shader.hpp"
#include "shapes/Rectangle2D.hpp"
#include "texture/Texture2D.hpp"

struct RenderTexture2D {
  RenderTexture2D() = default;


  RenderTexture2D(const TextureDescriptor& desc) : texture(desc) {
    destroy();
    fbo.gen();
    fbo.attach2D(GL_COLOR_ATTACHMENT0, texture);
  }

  ~RenderTexture2D() {
    destroy();
  }

  RenderTexture2D& operator=(RenderTexture2D&& rhs) = default;

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

  void destroy() {
    if (fbo.id) {
      texture.destroy();
      fbo.destroy();
    }
  }

  Texture2D texture;
  FBO fbo;
};

