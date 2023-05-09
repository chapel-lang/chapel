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

#include "./misc.h"
#include "llvm/Support/raw_ostream.h"

namespace chpldef {

const char* jsonTagStr(const JsonValue& json) {
  switch (json.kind()) {
    case JsonValue::Null: return "null";
    case JsonValue::Boolean: return "boolean";
    case JsonValue::Number: return "number";
    case JsonValue::String: return "string";
    case JsonValue::Array: return "array";
    case JsonValue::Object: return "object";
  }
}

/** Print a JSON value. */
std::string jsonToString(const JsonValue& json, bool pretty) {
  if (pretty) CHPLDEF_TODO();
  std::string ret;
  llvm::raw_string_ostream stream(ret);
  stream << json;
  return ret;
}

} // end namespace 'chpldef'
