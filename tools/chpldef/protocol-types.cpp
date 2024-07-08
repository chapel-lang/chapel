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

#include "./protocol-types.h"
#include "./misc.h"
#include "llvm/Support/JSON.h"
#include <cstdlib>
#include <iostream>

template <typename T>
static void doAddField(chpldef::JsonObject& obj, const char* name,
                       const T& field) {
  obj[name] = field;
}

template <typename T>
static void doAddField(chpldef::JsonObject& obj, const char* name,
                       const chpldef::opt<T>& field) {
  if (field) obj[name] = field;
}

/** Helper to make populating JSON object fields less painful. */
#define FIELD_(obj__, name__) \
  do { doAddField(obj__, #name__, name__); } while (0)

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
  JsonObject ret;
  FIELD_(ret, capabilities);
  FIELD_(ret, serverInfo);
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
  JsonMapper m(j, p);
  return m && MAP_(m, uri) && MAP_(m, name);
}

JsonValue ServerInfo::toJson() const {
  JsonObject ret;
  FIELD_(ret, name);
  FIELD_(ret, version);
  return ret;
}

JsonValue ServerCapabilities::toJson() const {
  JsonObject ret;
  FIELD_(ret, positionEncoding);
  FIELD_(ret, textDocumentSync);
  FIELD_(ret, notebookDocumentSync);
  FIELD_(ret, completionProvider);
  FIELD_(ret, hoverProvider);
  FIELD_(ret, signatureHelpProvider);
  FIELD_(ret, declarationProvider);
  FIELD_(ret, definitionProvider);
  FIELD_(ret, typeDefinitionProvider);
  FIELD_(ret, implementationProvider);
  FIELD_(ret, referencesProvider);
  FIELD_(ret, documentHighlightProvider);
  FIELD_(ret, documentSymbolProvider);
  FIELD_(ret, codeActionProvider);
  FIELD_(ret, codeLensProvider);
  FIELD_(ret, documentLinkProvider);
  FIELD_(ret, colorProvider);
  FIELD_(ret, documentFormattingProvider);
  FIELD_(ret, documentRangeFormattingProvider);
  FIELD_(ret, documentOnTypeFormattingProvider);
  FIELD_(ret, renameProvider);
  FIELD_(ret, foldingRangeProvider);
  FIELD_(ret, executeCommandProvider);
  FIELD_(ret, selectionRangeProvider);
  FIELD_(ret, linkEditingRangeProvider);
  FIELD_(ret, callHierarchyProvider);
  FIELD_(ret, semanticTokensProvider);
  FIELD_(ret, monikerProvider);
  FIELD_(ret, typeHierarchyProvider);
  FIELD_(ret, inlineValueProvider);
  FIELD_(ret, inlayHintProvider);
  FIELD_(ret, diagnosticProvider);
  FIELD_(ret, workspaceSymbolProvider);
  FIELD_(ret, workspace);
  FIELD_(ret, experimental);
  return ret;
}

JsonValue TextDocumentSyncOptions::toJson() const {
  JsonObject ret;
  FIELD_(ret, openClose);
  if (change) ret["change"] = static_cast<int>(*change);
  return ret;
}

bool TextDocumentItem::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return MAP_(m, uri) && MAP_(m, languageId) && MAP_(m, version) &&
         MAP_(m, text);
}

bool DidOpenParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, textDocument);
}

bool DidSaveParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, textDocument);
}

bool DidCloseParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, textDocument);
}

bool DidChangeParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, textDocument);
}

JsonValue SaveOptions::toJson() const {
  JsonObject ret;
  FIELD_(ret, includeText);
  return ret;
}

bool TextDocumentIdentifier::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, uri);
}

bool Position::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, line) && MAP_(m, character);
}

JsonValue Position::toJson() const {
  JsonObject ret;
  FIELD_(ret, line);
  FIELD_(ret, character);
  return ret;
}

bool Position::operator==(const Position& rhs) const {
  return line == rhs.line && character == rhs.character;
}

bool Position::operator!=(const Position& rhs) const {
  return !(*this == rhs);
}

bool Position::operator<(const Position& rhs) const {
  return line < rhs.line || (line == rhs.line && character < rhs.character);
}

bool Position::operator<=(const Position& rhs) const {
  return (*this < rhs) || (*this == rhs);
}

bool Position::operator>(const Position& rhs) const {
  return !(*this <= rhs);
}

bool Position::operator>=(const Position& rhs) const {
  return (*this > rhs) || (*this == rhs);
}

bool TextDocumentPositionParams::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, textDocument) && MAP_(m, position);
}

Location::Location(const chpl::Location& loc)
    : uri(loc.path().c_str()),
      range(Range(loc)) {}

bool Location::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, uri) && MAP_(m, range);
}

JsonValue Location::toJson() const {
  JsonObject ret;
  FIELD_(ret, uri);
  FIELD_(ret, range);
  return ret;
}

bool Location::operator==(const Location& rhs) const {
  return uri == rhs.uri && range == rhs.range;
}

bool LocationLink::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, originSelectionRange) &&
      MAP_(m, targetUri) &&
      MAP_(m, targetRange) &&
      MAP_(m, targetSelectionRange);
}

JsonValue LocationLink::toJson() const {
  JsonObject ret;
  FIELD_(ret, originSelectionRange);
  FIELD_(ret, targetUri);
  FIELD_(ret, targetRange);
  FIELD_(ret, targetSelectionRange);
  return ret;
}

static Position constructPosition(int chapelLine, int chapelColumn) {
  int line = chapelLine - 1;
  int character = chapelColumn - 1;
  CHPL_ASSERT(line >= 0 && character >= 0);
  return Position(line, character);
}

static Position constructPositionAtStart(const chpl::Location& loc) {
  return constructPosition(loc.firstLine(), loc.firstColumn());
}

static Position constructPositionAtEnd(const chpl::Location& loc) {
  return constructPosition(loc.lastLine(), loc.lastColumn());
}

Range::Range(const chpl::Location& loc)
  : start(constructPositionAtStart(loc)),
    end(constructPositionAtEnd(loc)) {}

bool Range::fromJson(const JsonValue& j, JsonPath p) {
  JsonMapper m(j, p);
  return m && MAP_(m, start) && MAP_(m, end);
}

JsonValue Range::toJson() const {
  JsonObject ret;
  FIELD_(ret, start);
  FIELD_(ret, end);
  return ret;
}

bool Range::operator==(const Range& rhs) const {
  return start == rhs.start && end == rhs.end;
}

bool Range::operator!=(const Range& rhs) const {
  return !(*this == rhs);
}

bool Range::operator<(const Range& rhs) const {
  return start < rhs.start;
}

bool Range::operator<=(const Range& rhs) const {
  return (*this < rhs) || (*this == rhs);
}

bool Range::operator>(const Range& rhs) const {
  return end > rhs.end;
}

bool Range::operator>=(const Range& rhs) const {
  return (*this > rhs) || (*this == rhs);
}

bool Range::contains(const Range& r) const {
  return start <= r.start && end >= r.end;
}

bool Range::contains(const Position& p) const {
  return start <= p && end >= p;
}

bool Range::overlaps(const Range& r) const {
  return contains(r.start) || contains(r.end);
}

bool Range::isNegative() const {
  return end < start;
}

JsonValue DeclarationResult::toJson() const {
  if (!result) return nullptr;
  if (auto v = std::get_if<LocationArray>(&*result)) return *v;
  if (auto v = std::get_if<std::vector<LocationLink>>(&*result)) return *v;
  return nullptr;
}

JsonValue DefinitionResult::toJson() const {
  if (!result) return nullptr;
  if (auto v = std::get_if<std::vector<Location>>(&*result)) return *v;
  if (auto v = std::get_if<std::vector<LocationLink>>(&*result)) return *v;
  return nullptr;
}

} // end namespace 'chpldef'
