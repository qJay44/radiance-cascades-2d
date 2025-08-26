#include <cstddef>
#include <direct.h>

#include "engine/Rectangle2D.hpp"
#include "engine/Shader.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "rc/Texture.hpp"
#include "utils/clrp.hpp"
#include "engine/InputsHandler.hpp"
#include "engine/gui.hpp"
#include "rc/Config.hpp"

using global::window;

void GLAPIENTRY MessageCallback(
  GLenum source,
  GLenum type,
  GLuint id,
  GLenum severity,
  GLsizei length,
  const GLchar* message,
  const void* userParam
) {
  if (source == GL_DEBUG_SOURCE_SHADER_COMPILER) return; // Handled by the Shader class itself

  clrp::clrp_t clrpError;
  clrpError.attr = clrp::ATTRIBUTE::BOLD;
  clrpError.fg = clrp::FG::RED;
  fprintf(
    stderr, "GL CALLBACK: %s source = 0x%x, id = 0x%x type = 0x%x, severity = 0x%x, message = %s\n",
    (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), source, id, type, severity, clrp::format(message, clrpError).c_str()
  );
  exit(1);
}

int main() {
  // Assuming the executable is launching from its own directory
  _chdir("../../../src");

  // GLFW init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // Window init
  window = glfwCreateWindow(900, 900, "Title", NULL, NULL);
  ivec2 winSize;
  glfwGetWindowSize(window, &winSize.x, &winSize.y);

  if (!window) {
    printf("Failed to create GFLW window\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, InputsHandler::keyCallback);
  glfwSetScrollCallback(window, InputsHandler::scrollCallback);

  // GLAD init
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return EXIT_FAILURE;
  }

  // Setting the window
  glViewport(0, 0, winSize.x, winSize.y);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);

  // ImGui init
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();

  struct Avg {
    float ms = 0.f;
    size_t fps = 0;
    size_t frameIdx = 1;
  } avg;

  // ----- Shaders --------------------------------------------- //

  Shader shaderGenerateRC("generateRC.comp");
  Shader shaderScreen("screen.vert", "screen.frag");
  shaderScreen.setUniformTexture("u_rcTexture", 0);
  shaderScreen.setUniform1ui("u_cascadeIdx", 0);

  // ----- Cascades texture ------------------------------------ //

  rc::config.update();
  rc::Texture rcTexture("u_rcTexture", 0, rc::config.cascadeCount);

  // ----- Compute cascades ------------------------------------ //

  constexpr uvec2 localSize(16);
  const uvec2 numGroups = (rcTexture.getSize() + localSize - 1u) / localSize;

  shaderGenerateRC.use();
  rcTexture.bindImage(GL_WRITE_ONLY);
  glDispatchCompute(numGroups.x, numGroups.y, rc::config.cascadeCount);
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

  // ----------------------------------------------------------- //

  Rectangle2D screenRect(winSize);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    static double titleTime = glfwGetTime();
    static double prevTime = titleTime;
    static double currTime = prevTime + 1e-6f;

    constexpr double fpsLimit = 1. / 144.;
    currTime = glfwGetTime();
    global::dt = currTime - prevTime;

    // FPS cap
    if (global::dt < fpsLimit) continue;
    else prevTime = currTime;

    global::time += global::dt;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const size_t fps = static_cast<size_t>(1.f / global::dt);
    const float ms = global::dt * 1000.f;

    // Show average (from 90 frames) fps and frame time
    if (avg.frameIdx++ < 90) {
      avg.fps += fps;
      avg.ms += ms;
      glfwSetWindowTitle(window, std::format("FPS: {} / {:.2f} ms", avg.fps / avg.frameIdx, avg.ms / avg.frameIdx).c_str());
    } else {
      avg.fps /= avg.frameIdx;
      avg.ms /= avg.frameIdx;
      avg.frameIdx = 1;
      glfwSetWindowTitle(window, std::format("FPS: {} / {:.2f} ms", avg.fps, avg.ms).c_str());
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw stuff...
    rcTexture.bind();
    screenRect.draw(shaderScreen);
    rcTexture.unbind();

    gui::draw();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
}

