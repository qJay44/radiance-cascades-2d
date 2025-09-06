#pragma once

#include "ProfilerManager.hpp"

#include <chrono>

struct ScopedProfileTask {
  static ProfilerManager* profilerManager;
  size_t taskIdx;

  std::chrono::steady_clock::time_point start;

  ScopedProfileTask(const std::string& taskName);
  ~ScopedProfileTask();
};

