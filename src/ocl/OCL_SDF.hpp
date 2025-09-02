#pragma once

#include <vector>

#include "CL/cl.h"
#include "../engine/shapes/Rectangle2D.hpp"
#include "../engine/shapes/Circle2D.hpp"
#include "utils/types.hpp"

class OCL_SDF {
public:
  OCL_SDF(bool printInfo = false);
  ~OCL_SDF();

  void updateImage(uvec2 size);
  void updateCirclesBuffer(const std::vector<Circle2D>& circles);
  void updateRectsBuffer(const std::vector<Rectangle2D>& rects);

  void run();

  [[nodiscard]]
  const s16* getPixels() const;

private:
  uvec2 size;
  s16* sdfPixels = nullptr;

  struct CircleCL {
    cl_float2 center;
    cl_float radius;
  };

  struct RectangleCL {
    cl_float2 center;
    cl_float2 sizeFromCenter;
  };

  CircleCL* hostCircles = nullptr;
  cl_uint numCircles = 0;

  RectangleCL* hostRectangles = nullptr;
  cl_uint numRects = 0;

  cl_device_id device = nullptr;
  size_t maxLocalSize;
  size_t maxDimensions;

  cl_context context;
  cl_command_queue commandQueue;

  cl_mem gpuImage = nullptr;
  cl_mem gpuCircles = nullptr;
  cl_mem gpuRectangles = nullptr;

  cl_kernel kernel;
  cl_program program;

private:
  void createCirclesBuffer(int count);
  void createRectsBuffer(int count);

  void clearHostCircles();
  void clearHostRectangles();

  void clearGpuCircles();
  void clearGpuRectangles();
};

