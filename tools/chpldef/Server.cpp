/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "./Server.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/framework/query-impl.h"
#include "chpl/util/filesystem.h"
#include "chpl/util/printf.h"

#include "llvm/ADT/Optional.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"

#include <chrono>
#include <thread>

// Wrapper to forward printf arguments to logger.
#define VPRINTF_FORWARD_(fmt__, func__) \
  do { \
    va_list args__; \
    va_start(args__, fmt__); \
    func__(fmt__, args__); \
    va_end(args__); \
  } while (0)

namespace chpldef {

Server::Server() {}

void Server::setLogger(Logger&& logger) {
  this->logger_ = std::move(logger);
}

void Server::sleep(int msec) {
  CHPL_ASSERT(msec >= 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

void Server::message(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, logger_.vmessage);
}

void Server::verbose(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, logger_.vverbose);
}

void Server::trace(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, logger_.vtrace);
}

} // end namespace 'chpldef'
