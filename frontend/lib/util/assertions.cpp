/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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


#include "chpl/util/assertions.h"
#include "chpl/framework/Context.h"

namespace chpl {

// Whether or not to exit on assertion failure
bool assertionsAreFatal_ = true;
// Whether or not to enable assertions
bool assertionsAreOn_ = true;

void setAssertions(bool enable) {
  assertionsAreOn_ = enable;
}

void setAssertionsFatal(bool enable) {
  assertionsAreFatal_ = enable;
}

bool assertionsAreOn() {
  return assertionsAreOn_;
}

bool assertionsAreFatal() {
  return assertionsAreFatal_;
}

void assertion(bool expr, const char* filename, const char* func,
               int lineno, const char* exprText) {
  // assertions OFF: skip all checks
  if (!assertionsAreOn()) {
    return;
  }

  // assertions ON: perform check, conditionally exit if assertionsAreFatal
  if (!expr) {
    // assertion failed: write out error
    fprintf(stderr, "[%s:%d in %s] Assertion failed: %s\n",
            filename, lineno, func, exprText);
    if (assertionsAreFatal()) {
      std::abort();
    }
  }
  // assertions active and passed
}

void chpl_unimpl(const char* filename, const char* func, int lineno,
                 const char* msg) {
  std::string fname(filename);
  std::string shortName = fname;
  auto frontPos = fname.find("frontend");
  if (frontPos != std::string::npos) {
    shortName = fname.substr(frontPos, std::string::npos);
  }
  fprintf(stderr, "[%s:%d in %s] Unimplemented: %s\n",
                  shortName.c_str(), lineno, func, msg);
};


} // end namespace chpl
