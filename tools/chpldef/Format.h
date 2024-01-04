/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_TOOLS_CHPLDEF_FORMAT_H
#define CHPL_TOOLS_CHPLDEF_FORMAT_H

#include "Message.h"
#include "chpl/uast/AstNode.h"
#include "chpl/framework/Location.h"
#include "chpl/framework/UniqueString.h"
#include <sstream>

namespace chpldef {

class Format {
public:
  enum Detail {
    DEFAULT = 0
  };
private:
  std::stringstream ss_;
  Server* ctx_;
  Detail dt_;

  void writeit(const chpl::uast::AstNode& t);
  void writeit(const chpl::Location& t);
  void writeit(const chpldef::Message& t);
public:
  Format(Server* ctx, Detail dt) : ctx_(ctx), dt_(dt) {}
 ~Format() = default;

  inline std::string read() const { return ss_.str(); }

  template <typename T>
  void write(const T& t) {
    std::ignore = dt_; writeit(t);
  }

  template <typename T>
  void write(T* const& t) {
    if (t == nullptr) { ss_ << "<null>"; return; }
    auto& tx = *t; write(tx);
  }
};

} // end namespace 'chpldef'

#endif
