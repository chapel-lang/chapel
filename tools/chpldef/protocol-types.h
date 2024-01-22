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

#ifndef CHPL_TOOLS_CHPLDEF_PROTOCOL_TYPES_H
#define CHPL_TOOLS_CHPLDEF_PROTOCOL_TYPES_H

#include "Logger.h"
#include "misc.h"
#include <cstdint>
#include <variant>
#include <vector>

/** This header contains types which help form the Microsoft language server
    protocol. The types attempt to follow the specification as faithfully
    as possible, but liberty is taken in cases where definitions are too
    deeply nested or otherwise redundant.

    NOTE: If you define a new struct in this file, you should add a pair of
    (de)serializer functions to the section at the bottom of the file in
    order to define how the type is converted to/from JSON.
*/
namespace chpldef {

/** Forward declare request params/result types, even if they may not all
    be defined (e.g., a notification does not have a result). */
#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
  struct name__##Params; \
  struct name__##Result;
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

using OPT_TODO_TYPE = opt<int>;

struct BaseProtocolType {
  virtual bool fromJson(const JsonValue& j, JsonPath p);
  virtual JsonValue toJson() const;

  /** By default, convert to JSON and then print the JSON. */
  virtual std::string toString() const;
  virtual ~BaseProtocolType() = default;
};

struct EmptyProtocolType : BaseProtocolType {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
  virtual JsonValue toJson() const override;
  virtual ~EmptyProtocolType() = default;
};

struct ProtocolTypeSend : BaseProtocolType {
  virtual JsonValue toJson() const override = 0;
  virtual ~ProtocolTypeSend() = default;
};

struct ProtocolTypeRecv : BaseProtocolType {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override = 0;
  virtual ~ProtocolTypeRecv() = default;
};

struct ProtocolType : BaseProtocolType {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override = 0;
  virtual JsonValue toJson() const override = 0;
  virtual ~ProtocolType() = default;
};

struct ClientInfo : ProtocolTypeRecv {
  std::string name;
  opt<std::string> version;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

/** TODO: Used to store 'chpldef' specific initialization options. */
struct ChpldefInit : ProtocolTypeRecv {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

/** As defined by the spec, this structure is deeply nested and absolutely
    impossible to read/understand. I've opted to follow clangd's lead here
    and only list things we should (probably) care about.

    For right now, this struct is empty (because the spec allows all the
    fields to be nullable).

    TODO: If you add a field here, then adjust the (de)serializer methods.
*/
struct ClientCapabilities : ProtocolTypeRecv {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct WorkspaceFolder : ProtocolTypeRecv {
  std::string uri;
  std::string name;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct TraceLevel : ProtocolTypeRecv {
  Logger::Level level;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct InitializeParams : ProtocolTypeRecv {
  opt<int64_t> processId;
  opt<ClientInfo> clientInfo;
  opt<std::string> locale;
  opt<std::string> rootPath;  /** Deprecated -> 'rootUri'. */
  opt<std::string> rootUri;   /** Deprecated -> 'workspaceFolders. */
  opt<ChpldefInit> initializationOptions;
  ClientCapabilities capabilities;
  opt<TraceLevel> trace;
  opt<std::vector<WorkspaceFolder>> workspaceFolders;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct SaveOptions : ProtocolTypeSend {
  opt<bool> includeText;

  virtual JsonValue toJson() const override;
};

struct TextDocumentSyncOptions : ProtocolTypeSend {
  enum Change {
    None          = 0,
    Full          = 1,
    Incremental   = 2
  };
  opt<bool> openClose;
  opt<Change> change;
  opt<bool> willSave;
  opt<bool> willSaveWaitUntil;
  opt<SaveOptions> save;

  virtual JsonValue toJson() const override;
};

/** Some of the 'provider' queries have more advanced types we can swap
    in to configure further -- see 'DeclarationRegistrationOptions'. */
struct ServerCapabilities : ProtocolTypeSend {
  opt<std::string> positionEncoding;
  opt<TextDocumentSyncOptions> textDocumentSync;
  OPT_TODO_TYPE notebookDocumentSync;
  OPT_TODO_TYPE completionProvider;
  opt<bool> hoverProvider;
  OPT_TODO_TYPE signatureHelpProvider;
  opt<bool> declarationProvider;
  opt<bool> definitionProvider;
  opt<bool> typeDefinitionProvider;
  opt<bool> implementationProvider;
  opt<bool> referencesProvider;
  opt<bool> documentHighlightProvider;
  opt<bool> documentSymbolProvider;
  opt<bool> codeActionProvider;
  OPT_TODO_TYPE codeLensProvider;
  OPT_TODO_TYPE documentLinkProvider;
  opt<bool> colorProvider;
  opt<bool> documentFormattingProvider;
  opt<bool> documentRangeFormattingProvider;
  OPT_TODO_TYPE documentOnTypeFormattingProvider;
  opt<bool> renameProvider;
  opt<bool> foldingRangeProvider;
  OPT_TODO_TYPE executeCommandProvider;
  opt<bool> selectionRangeProvider;
  opt<bool> linkEditingRangeProvider;
  opt<bool> callHierarchyProvider;
  OPT_TODO_TYPE semanticTokensProvider;
  opt<bool> monikerProvider;
  opt<bool> typeHierarchyProvider;
  opt<bool> inlineValueProvider;
  opt<bool> inlayHintProvider;
  OPT_TODO_TYPE diagnosticProvider;
  opt<bool> workspaceSymbolProvider;
  OPT_TODO_TYPE workspace;
  OPT_TODO_TYPE experimental;

  virtual JsonValue toJson() const override;
};

struct ServerInfo : ProtocolTypeSend {
  std::string name;
  opt<std::string> version;

  virtual JsonValue toJson() const override;
};

struct InitializeResult : ProtocolTypeSend {
  ServerCapabilities capabilities;
  opt<ServerInfo> serverInfo;

  virtual JsonValue toJson() const override;
};

struct InitializedParams : EmptyProtocolType {};
struct ShutdownParams : EmptyProtocolType {};
struct ShutdownResult : EmptyProtocolType {};
struct ExitParams : EmptyProtocolType {};

struct TextDocumentItem : ProtocolTypeRecv {
  std::string uri;
  std::string languageId;
  int64_t version = -1;
  std::string text;

  TextDocumentItem() = default;
  TextDocumentItem(std::string uri, std::string languageId, int64_t version,
                   std::string text)
    : uri(std::move(uri)),
      languageId(std::move(languageId)),
      version(version),
      text(std::move(text)) {}
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct DidOpenParams : ProtocolTypeRecv {
  TextDocumentItem textDocument;

  DidOpenParams() = default;
  DidOpenParams(TextDocumentItem textDocument)
    : textDocument(std::move(textDocument)) {}
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct DidChangeParams : ProtocolTypeRecv {
  TextDocumentItem textDocument;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct DidSaveParams : ProtocolTypeRecv {
  TextDocumentItem textDocument;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct DidCloseParams : ProtocolTypeRecv {
  TextDocumentItem textDocument;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct TextDocumentIdentifier : ProtocolTypeRecv {
  std::string uri;

  TextDocumentIdentifier() = default;
  TextDocumentIdentifier(std::string uri) : uri(std::move(uri)) {}
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct Position : ProtocolType {
  uint64_t line = 0;            /** Zero-based position. */
  uint64_t character = 0;       /** Zero-based position. */

  Position() = default;
  Position(uint64_t line, uint64_t character)
    : line(line), character(character) {}
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
  virtual JsonValue toJson() const override;
  bool operator==(const Position& rhs) const;
  bool operator!=(const Position& rhs) const;
  bool operator<(const Position& rhs) const;
  bool operator<=(const Position& rhs) const;
  bool operator>(const Position& rhs) const;
  bool operator>=(const Position& rhs) const;
};

struct Range : ProtocolType {
  Position start;
  Position end;

  Range() = default;
  Range(Position start, Position end)
    : start(std::move(start)),
      end(std::move(end)) {}
  Range(const chpl::Location& loc);
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
  virtual JsonValue toJson() const override;
  bool operator==(const Range& rhs) const;
  bool operator!=(const Range& rhs) const;

  /** Determine if the start position of this is less than that of 'rhs'. */
  bool operator<(const Range& rhs) const;
  bool operator<=(const Range& rhs) const;

  /** Determine if the end position of this is greater than that of 'rhs'. */
  bool operator>(const Range& rhs) const;
  bool operator>=(const Range& rhs) const;

  /** Determine if this contains 'r'. Equal ranges contain each other. */
  bool contains(const Range& r) const;

  /** Determine if this contains the position 'p'. */
  bool contains(const Position& p) const;

  /** Determine if one range overlaps another. */
  bool overlaps(const Range& r) const;

  /** Determine if the end position is less than the start position. */
  bool isNegative() const;
};

struct TextDocumentPositionParams : ProtocolTypeRecv {
  TextDocumentIdentifier textDocument;
  Position position;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
};

struct Location : ProtocolType {
  std::string uri;
  Range range;

  Location(std::string uri, Range range)
    : uri(std::move(uri)),
      range(std::move(range)) {}
  Location() = default;
  Location(const chpl::Location& loc);
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
  virtual JsonValue toJson() const override;
  bool operator==(const Location& rhs) const;
  bool operator!=(const Location& rhs) const;
};

struct LocationLink : ProtocolType {
  opt<Range> originSelectionRange;
  std::string targetUri;
  Range targetRange;
  Range targetSelectionRange;

  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
  virtual JsonValue toJson() const override;
};

using LocationArray = std::vector<Location>;
using LocationLinkArray = std::vector<LocationLink>;

struct DeclarationParams : TextDocumentPositionParams {};
struct DefinitionParams : TextDocumentPositionParams {};

struct DeclarationResult : ProtocolTypeSend {
  opt<std::variant<LocationArray, LocationLinkArray>> result;

  virtual JsonValue toJson() const override;
};

struct DefinitionResult : ProtocolTypeSend {
  opt<std::variant<LocationArray, LocationLinkArray>> result;

  virtual JsonValue toJson() const override;
};

/** Instantiate only if 'T' is derived from 'ProtocolType'. */
template <typename T>
CHPLDEF_ENABLE_IF_DERIVED(T, BaseProtocolType, bool)
fromJSON(const JsonValue& j, T& x, JsonPath p) {
  return x.fromJson(j, p);
}

/** Instantiate only if 'T' is derived from 'ProtocolType'. */
template <typename T>
CHPLDEF_ENABLE_IF_DERIVED(T, BaseProtocolType, JsonValue)
toJSON(const T& x) {
  return x.toJson();
}

} // end namespace 'chpldef'

#endif
