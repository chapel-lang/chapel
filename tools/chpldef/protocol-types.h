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

#ifndef CHPL_TOOLS_CHPLDEF_PROTOCOL_TYPES_H
#define CHPL_TOOLS_CHPLDEF_PROTOCOL_TYPES_H

#include "./Logger.h"
#include "./misc.h"
#include <cstdint>

/** This header contains types which help form the Microsoft language server
    protocol. The types attempt to follow the specification as faithfully
    as possible, but liberty is taken in cases where definitions are too
    deeply nested or otherwise redundant.

    NOTE: If you define a new struct in this file, you should add a pair of
    (de)serializer functions to the section at the bottom of the file in
    order to define how the type is converted to/from JSON.
*/
namespace chpldef {

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
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;

  std::string name;
  opt<std::string> version;
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
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;

  std::string uri;
  std::string name;
};

struct TraceLevel : ProtocolTypeRecv {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;

  Logger::Level level;
};

struct InitializeParams : ProtocolTypeRecv {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;

  opt<int64_t> processId;
  opt<ClientInfo> clientInfo;
  opt<std::string> locale;
  opt<std::string> rootPath;  /** Deprecated -> 'rootUri'. */
  opt<std::string> rootUri;   /** Deprecated -> 'workspaceFolders. */
  opt<ChpldefInit> initializationOptions;
  ClientCapabilities capabilities;
  opt<TraceLevel> trace;
  opt<std::vector<WorkspaceFolder>> workspaceFolders;
};

struct TextDocumentSyncOptions : ProtocolTypeSend {
  virtual JsonValue toJson() const override;

  /** Valid 'change' values. */
  enum Change {
    None          = 0,
    Full          = 1,
    Incremental   = 2
  };

  opt<bool> openClose;
  opt<Change> change;
};

/** Some of the 'provider' queries have more advanced types we can swap
    in to configure further -- see 'DeclarationRegistrationOptions'. */
struct ServerCapabilities : ProtocolTypeSend {
  virtual JsonValue toJson() const override;

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
};

struct ServerInfo : ProtocolTypeSend {
  virtual JsonValue toJson() const override;

  std::string name;
  opt<std::string> version;
};

struct InitializeResult : ProtocolTypeSend {
  virtual JsonValue toJson() const override;

  ServerCapabilities capabilities;
  opt<ServerInfo> serverInfo;
};

struct InitializedParams : EmptyProtocolType {};
struct InitializedResult : EmptyProtocolType {};

struct ShutdownParams : EmptyProtocolType {};
struct ShutdownResult : EmptyProtocolType {};

struct ExitParams : EmptyProtocolType {};
struct ExitResult : EmptyProtocolType {};

struct TextDocumentItem : ProtocolTypeRecv {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;

  std::string uri;
  std::string languageId;
  int64_t version;
  std::string text;
};

struct DidOpenResult : EmptyProtocolType {};
struct DidOpenParams : ProtocolTypeRecv {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;

  TextDocumentItem textDocument;
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
