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
#include <type_traits>

/** Bunch up some redundant overrides for protocol structs into a macro. */
#define CHPLDEF_PROTOCOL_TYPE_OVERRIDES() \
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;  \
  virtual JsonValue toJson() const override;

/** Use this to declare protocol types that are empty. */
#define CHPLDEF_PROTOCOL_EMPTY_TYPE(name__) \
  struct name__ : EmptyProtocolType {}

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

struct ProtocolType {
  virtual bool fromJson(const JsonValue& j, JsonPath p) = 0;
  virtual JsonValue toJson() const = 0;

  /** By default, convert to JSON and then print the JSON. */
  virtual std::string toString() const;
  virtual ~ProtocolType() = default;
};

struct EmptyProtocolType : ProtocolType {
  virtual bool fromJson(const JsonValue& j, JsonPath p) override;
  virtual JsonValue toJson() const override;
  virtual ~EmptyProtocolType() = default;
};

/** Information about the client. */
struct ClientInfo : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  std::string name;
  opt<std::string> version;
};

/** TODO: Used to store 'chpldef' specific initialization options. */
struct ChpldefInit : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();
};

/** As defined by the spec, this structure is deeply nested and absolutely
    impossible to read/understand. I've opted to follow clangd's lead here
    and only list things we should (probably) care about.

    For right now, this struct is empty (because the spec allows all the
    fields to be nullable).

    TODO: If you add a field here, then adjust the (de)serializer methods.
*/
struct ClientCapabilities : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();
};

struct WorkspaceFolder : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  std::string uri;
  std::string name;
};

struct TraceLevel : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  Logger::Level level;
};

struct InitializeParams : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  opt<int> processId;
  opt<ClientInfo> clientInfo;
  opt<std::string> locale;
  opt<std::string> rootPath;  /** Deprecated -> 'rootUri'. */
  opt<std::string> rootUri;   /** Deprecated -> 'workspaceFolders. */
  opt<ChpldefInit> initializationOptions;
  ClientCapabilities capabilities;
  opt<TraceLevel> trace;
  opt<std::vector<WorkspaceFolder>> workspaceFolders;
};

struct TextDocumentSyncOptions : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();
};

/** Some of the 'provider' queries have more advanced types we can swap
    in to configure further -- see 'DeclarationRegistrationOptions'. */
struct ServerCapabilities : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  opt<std::string> positionEncoding;
  OPT_TODO_TYPE textDocumentSync;
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

struct ServerInfo : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  std::string name;
  opt<std::string> version;
};

struct InitializeResult : ProtocolType {
  CHPLDEF_PROTOCOL_TYPE_OVERRIDES();

  ServerCapabilities capabilities;
  opt<ServerInfo> serverInfo;
};

CHPLDEF_PROTOCOL_EMPTY_TYPE(InitializedParams);
CHPLDEF_PROTOCOL_EMPTY_TYPE(InitializedResult);

CHPLDEF_PROTOCOL_EMPTY_TYPE(ShutdownParams);
CHPLDEF_PROTOCOL_EMPTY_TYPE(ShutdownResult);

CHPLDEF_PROTOCOL_EMPTY_TYPE(ExitParams);
CHPLDEF_PROTOCOL_EMPTY_TYPE(ExitResult);

/** Instantiate only if 'T' is derived from 'ProtocolType'. */
template <typename T>
CHPLDEF_ENABLE_IF_DERIVED(T, ProtocolType, bool)
fromJSON(const JsonValue& j, T& x, JsonPath p) {
  return x.fromJson(j, p);
}

/** Instantiate only if 'T' is derived from 'ProtocolType'. */
template <typename T>
CHPLDEF_ENABLE_IF_DERIVED(T, ProtocolType, JsonValue)
toJSON(const T& x) {
  return x.toJson();
}

} // end namespace 'chpldef'

#endif
