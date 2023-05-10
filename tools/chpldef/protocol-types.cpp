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

#include "./protocol-types.h"
#include "./misc.h"
#include "llvm/Support/JSON.h"

namespace chpldef {

std::string ProtocolStruct::toString() const {
  auto ret = jsonToString(toJson());
  return ret;
}

bool ClientInfo::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  if (!m) return true;
  m.map("name", name);
  m.map("version", version);
  return false;
}

JsonValue ClientInfo::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool ChpldefInit::fromJson(const JsonValue& j, JsonPath p) {
  return false;
}

JsonValue ChpldefInit::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool ClientCapabilities::fromJson(const JsonValue& j, JsonPath p) {
  return false;
}

JsonValue ClientCapabilities::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool InitializeParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  if (!m) return true;
  m.map("processId", processId);
  m.map("clientInfo", clientInfo);
  m.map("locale", locale);
  m.map("rootPath", rootPath);
  m.map("rootUri", rootUri);
  m.map("initializationOptions", initializationOptions);
  m.map("capabilities", capabilities);
  m.map("trace", trace);
  m.map("workspaceFolders", workspaceFolders);
  return false;
}

JsonValue InitializeParams::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool InitializeResult::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_IMPOSSIBLE();
  return false;
}

JsonValue InitializeResult::toJson() const {
  CHPLDEF_TODO();
  return nullptr;
}

bool TraceLevel::fromJson(const JsonValue& j, JsonPath p) {
  if (auto s = j.getAsString()) {
    if (*s == "off") {
      level = Logger::OFF;
      return false;
    } else if (*s == "messages") {
      level = Logger::MESSAGES;
      return false;
    } else if (*s == "verbose") {
      level = Logger::VERBOSE;
      return false;
    }
  }
  return true;
}

JsonValue TraceLevel::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool WorkspaceFolder::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_TODO();
  return false;
}

JsonValue WorkspaceFolder::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool ServerInfo::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_IMPOSSIBLE();
  return false;
}

JsonValue ServerInfo::toJson() const {
  JsonObject ret {{ "name", name }, { "version", version }};
  return ret;
}

bool ServerCapabilities::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_IMPOSSIBLE();
  return false;
}

JsonValue ServerCapabilities::toJson() const {
  JsonValue ret(nullptr);
  return ret;
}

} // end namespace 'chpldef'

namespace llvm {
namespace json {

  bool fromJSON(const llvm::json::Value& j, chpldef::ProtocolStruct& x,
                llvm::json::Path p) {
    bool error = x.fromJson(j, p);
    if (error) return false;
    return true;
  }

  llvm::json::Value toJSON(const chpldef::ProtocolStruct& x) {
    auto ret = x.toJson();
    return ret;
  }

} // end namespace 'json'
} // end namespace 'llvm'
