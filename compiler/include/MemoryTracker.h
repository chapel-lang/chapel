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

#ifndef _chpl_MemoryTracker_h_
#define _chpl_MemoryTracker_h_

#include <cstdint>

class MemoryTracker {
public:
  static bool platformSupportsMemoryTracking();
  using MemoryInBytes = int64_t;
  using MemoryInMB = double;
  static MemoryInMB toMB(MemoryInBytes bytes) {
    return bytes / (1024.0 * 1024.0);
  }

  MemoryTracker() { clear(); }
  ~MemoryTracker() = default;

  void clear();
  void start();
  void stop();

  MemoryInBytes usedBytes() const;

private:
  MemoryInBytes mRefMemoryUsage;
  MemoryInBytes mAccumMemoryUsage;
  bool          mRunning;
};

#endif
