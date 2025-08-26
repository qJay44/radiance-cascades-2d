#pragma once

namespace global {

static const union { vec3 right   {1.f, 0.f, 0.f}, red;   };
static const union { vec3 up      {0.f, 1.f, 0.f}, green; };
static const union { vec3 forward {0.f, 0.f, 1.f}, blue;  };

extern GLFWwindow* window;

extern float dt;
extern float time;

}// global

