/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/StringLikeLiteral.h"

#include "chpl/uast/Builder.h"
#include "chpl/util/string-escapes.h"

namespace chpl {
namespace uast {


void StringLikeLiteral::dumpFieldsInner(const DumpSettings& s) const {
  const char* q = "\"";
  const char* suffix = "";

  switch (quotes_) {
    case StringLikeLiteral::SINGLE:
      q = "'";
      break;
    case StringLikeLiteral::DOUBLE:
      break;
    case StringLikeLiteral::TRIPLE_SINGLE:
      q = "'";
      suffix = " triple-single";
      break;
    case StringLikeLiteral::TRIPLE_DOUBLE:
      suffix = " triple-double";
      break;
  }

  s.out << q
        << escapeStringC(value().str())
        << q
        << suffix;
}

StringLikeLiteral::~StringLikeLiteral() {
}

const char* StringLikeLiteral::quoteStyleToString(QuoteStyle q) {
  switch (q) {
    case StringLikeLiteral::SINGLE:        return "'";
    case StringLikeLiteral::DOUBLE:        return "\"";
    case StringLikeLiteral::TRIPLE_SINGLE: return "'''";
    case StringLikeLiteral::TRIPLE_DOUBLE: return "\"\"\"";
  }

  CHPL_ASSERT(false && "should not be reached");
  return "";
}


} // namespace uast
} // namespace chpl
