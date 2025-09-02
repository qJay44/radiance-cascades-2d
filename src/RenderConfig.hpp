#pragma once

#include "engine/Shader.hpp"
#include "engine/texture/Texture2D.hpp"
#include "engine/FBO.hpp"
#include "ShapeContainer.hpp"
#include "global.hpp"
#include "ocl/OCL_SDF.hpp"
#include "rc/Config.hpp"
#include "utils/utils.hpp"

struct RenderConfig {
  Shader shaderRC = Shader("shape2D.vert", "rc.frag");
  Shader shaderShape2D = Shader("shape2D.vert", "shape2D.frag");
  OCL_SDF ocl;
  FBO fboShapes = FBO(1);

  Rectangle2D screenRect;
  ShapeContainer shapeContainer;
  Texture2D sdfTexture;
  Texture2D sceneTexture;

  RenderConfig() {};

  void init() {
    ivec2 winSize = getWinSize(global::window);

    ocl.updateImage(winSize);
    screenRect = Rectangle2D(winSize, winSize / 2);
    shapeContainer = ShapeContainer();

    sdfTexture.clear();
    sceneTexture.clear();
    sdfTexture = Texture2D({"u_sdfTexture", winSize, 0, GL_TEXTURE_2D, GL_R16F, GL_RED, GL_SHORT});
    sceneTexture = Texture2D({"u_sceneTexture", winSize, 1});

    fboShapes.attach2D(GL_COLOR_ATTACHMENT0, sceneTexture);

    shaderRC.setUniformTexture(sdfTexture);
    shaderRC.setUniformTexture(sceneTexture);

    rc::config.calcCascadeCount();
  }

  void update() {
    ocl.updateCirclesBuffer(shapeContainer.circles);
    ocl.updateRectsBuffer(shapeContainer.rects);
    ocl.run();
    sdfTexture.update(ocl.getPixels());
  }

  void bindTextures() const {
    sdfTexture.bind();
    sceneTexture.bind();
  }

  void unbindTextures() const {
    sdfTexture.unbind();
    sceneTexture.unbind();
  }

  void drawToShapesFBO() const {
    fboShapes.bind();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    shapeContainer.draw(shaderShape2D);
  }

  void drawToScreen() const {
    FBO::unbind();
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    bindTextures();
    screenRect.draw(shaderRC);
    unbindTextures();
  }
};

