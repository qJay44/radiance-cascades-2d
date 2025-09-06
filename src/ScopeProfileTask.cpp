#include "ScopeProfileTask.hpp"
#include <cassert>

ProfilerManager* ScopedProfileTask::profilerManager = nullptr;

ScopedProfileTask::ScopedProfileTask(const std::string& taskName)
  : start(std::chrono::steady_clock::now()) {
  assert(profilerManager);
  taskIdx = profilerManager->startTask(taskName);
}

ScopedProfileTask::~ScopedProfileTask() {
  assert(profilerManager);

  using namespace std::chrono;

  auto end = steady_clock::now();
  auto dur = end - start;
  float durationSec = duration_cast<duration<float>>(dur).count();

  profilerManager->endTask(taskIdx, durationSec);
}

