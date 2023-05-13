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
#include <cstdlib>
#include <iostream>

/** Helper to make populating JSON object fields less painful. */
#define FIELD_(name__) { #name__, name__ }

/** Helper to make reading JSON object fields less painful. */
#define MAP_(m__, name__) (m__.map(#name__, name__))

namespace chpldef {

bool BaseProtocolType::fromJson(const JsonValue& j, JsonPath p) {
  std::cerr << "Called 'BaseProtocolTypefromJson'!" << std::endl;
  std::abort();
  return false;
}

JsonValue BaseProtocolType::toJson() const {
  std::cerr << "Called 'BaseProtocolType::toJson'!" << std::endl;
  std::abort();
  return nullptr;
}

std::string BaseProtocolType::toString() const {
  auto ret = jsonToString(toJson());
  return ret;
}

bool EmptyProtocolType::fromJson(const JsonValue& j, JsonPath p) {
  return true;
}

JsonValue EmptyProtocolType::toJson() const {
  return nullptr;
}

bool ClientInfo::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  if (!m) return false;
  m.map("name", name);
  m.map("version", version);
  return true;
}

bool ChpldefInit::fromJson(const JsonValue& j, JsonPath p) {
  return true;
}

bool ClientCapabilities::fromJson(const JsonValue& j, JsonPath p) {
  return true;
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

bool WorkspaceFolder::fromJson(const JsonValue& j, JsonPath p) {
  CHPLDEF_TODO();
  return true;
}

JsonValue ServerInfo::toJson() const {
  JsonObject ret { FIELD_(name), FIELD_(version) };
  return ret;
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

JsonValue TextDocumentSyncOptions::toJson() const {
  JsonObject ret;
  if (openClose) ret["openClose"] = *openClose;
  if (change) ret["change"] = static_cast<int>(*change);
  return ret;
}

bool TextDocumentItem::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  bool ret = m;
  ret &= MAP_(m, uri);
  ret &= MAP_(m, languageId);
  ret &= MAP_(m, version);
  ret &= MAP_(m, text);
  return ret;
}

bool DidOpenParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, textDocument);
}

} // end namespace 'chpldef'
