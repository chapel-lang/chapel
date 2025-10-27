/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MemoryTracker.h"

#include <cstdlib>
#include <cstdio>

#include <sys/resource.h>
#include <unistd.h>
#if __APPLE__
#include <mach/mach.h>
#endif

#include "misc.h"

bool MemoryTracker::platformSupportsMemoryTracking() {
#ifdef __linux__
  return true;
#else
#if __APPLE__
  return false;
#else
  return false;
#endif
#endif
}
static MemoryTracker::MemoryInBytes getCurrentMemoryUsage() {
#ifdef __linux__
  FILE* fp = fopen("/proc/self/status", "r");
  if (fp == NULL) {
    return -1;
  }
  char line[256];
  long data = -1;
  while (fgets(line, sizeof(line), fp)) {
    if (sscanf(line, "VmData: %ld kB", &data) == 1) {
      break;
    }
  }
  fclose(fp);

  return data * 1024; // convert kB to bytes
#else
#if __APPLE__
  struct mach_task_basic_info t_info;
  mach_msg_type_number_t t_info_count = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(),
                MACH_TASK_BASIC_INFO,
                (task_info_t)&t_info,
                &t_info_count) != KERN_SUCCESS) {
    return -1;
  }
  return t_info.resident_size_max;
#else
  // unsupported
  return 0;
#endif
#endif
}

void MemoryTracker::clear() {
  mRefMemoryUsage   = 0;
  mAccumMemoryUsage = 0;
  mRunning          = false;
}
void MemoryTracker::start() {
  INT_ASSERT(!mRunning);
  mRefMemoryUsage = getCurrentMemoryUsage();
  mRunning        = true;
}
void MemoryTracker::stop() {
  INT_ASSERT(mRunning);
  mAccumMemoryUsage = usedBytes();
  mRunning          = false;
}

MemoryTracker::MemoryInBytes MemoryTracker::usedBytes() const {
  if (!mRunning) {
    return mAccumMemoryUsage;
  } else {
    auto current = getCurrentMemoryUsage();
    return current - mRefMemoryUsage;
  }
}
