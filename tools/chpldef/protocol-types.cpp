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

/** Helper to make populating JSON object fields less painful. */
#define FIELD_(name__) { #name__, name__ }

namespace chpldef {

std::string ProtocolType::toString() const {
  auto ret = jsonToString(toJson());
  return ret;
}

bool ClientInfo::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  if (!m) return false;
  m.map("name", name);
  m.map("version", version);
  return true;
}

JsonValue ClientInfo::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool ChpldefInit::fromJson(const JsonValue& j, JsonPath p) {
  return true;
}

JsonValue ChpldefInit::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool ClientCapabilities::fromJson(const JsonValue& j, JsonPath p) {
  return true;
}

JsonValue ClientCapabilities::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool InitializeParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  if (!m) return false;
  m.map("processId", processId);
  m.map("clientInfo", clientInfo);
  m.map("locale", locale);
  m.map("rootPath", rootPath);
  m.map("rootUri", rootUri);
  m.map("initializationOptions", initializationOptions);
  m.map("capabilities", capabilities);
  m.map("trace", trace);
  m.map("workspaceFolders", workspaceFolders);
  return true;
}

JsonValue InitializeParams::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool InitializeResult::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_IMPOSSIBLE();
  return true;
}

JsonValue InitializeResult::toJson() const {
  JsonObject ret {
    FIELD_(capabilities),
    FIELD_(serverInfo)
  };
  return ret;
}

bool TraceLevel::fromJson(const JsonValue& j, JsonPath p) {
  if (auto s = j.getAsString()) {
    if (*s == "off") {
      level = Logger::OFF;
      return true;
    } else if (*s == "messages") {
      level = Logger::MESSAGES;
      return true;
    } else if (*s == "verbose") {
      level = Logger::VERBOSE;
      return true;
    }
  }
  return false;
}

JsonValue TraceLevel::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool WorkspaceFolder::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_TODO();
  return true;
}

JsonValue WorkspaceFolder::toJson() const {
  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

bool ServerInfo::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_IMPOSSIBLE();
  return true;
}

JsonValue ServerInfo::toJson() const {
  JsonObject ret { FIELD_(name), FIELD_(version) };
  return ret;
}

bool ServerCapabilities::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_IMPOSSIBLE();
  return true;
}

JsonValue ServerCapabilities::toJson() const {
  JsonObject ret {
    FIELD_(positionEncoding),
    FIELD_(textDocumentSync),
    FIELD_(notebookDocumentSync),
    FIELD_(completionProvider),
    FIELD_(hoverProvider),
    FIELD_(signatureHelpProvider),
    FIELD_(declarationProvider),
    FIELD_(definitionProvider),
    FIELD_(typeDefinitionProvider),
    FIELD_(implementationProvider),
    FIELD_(referencesProvider),
    FIELD_(documentHighlightProvider),
    FIELD_(documentSymbolProvider),
    FIELD_(codeActionProvider),
    FIELD_(codeLensProvider),
    FIELD_(documentLinkProvider),
    FIELD_(colorProvider),
    FIELD_(documentFormattingProvider),
    FIELD_(documentRangeFormattingProvider),
    FIELD_(documentOnTypeFormattingProvider),
    FIELD_(renameProvider),
    FIELD_(foldingRangeProvider),
    FIELD_(executeCommandProvider),
    FIELD_(selectionRangeProvider),
    FIELD_(linkEditingRangeProvider),
    FIELD_(callHierarchyProvider),
    FIELD_(semanticTokensProvider),
    FIELD_(monikerProvider),
    FIELD_(typeHierarchyProvider),
    FIELD_(inlineValueProvider),
    FIELD_(inlayHintProvider),
    FIELD_(diagnosticProvider),
    FIELD_(workspaceSymbolProvider),
    FIELD_(workspace),
    FIELD_(experimental)
  };
  return ret;
}

} // end namespace 'chpldef'
