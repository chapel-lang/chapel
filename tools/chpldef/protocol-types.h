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

/** Bunch up some redundant overrides for protocol structs into a macro. */
#define CHPLDEF_PROTOCOL_STRUCT_OVERRIDES()               \
  virtual bool fromJson(const JsonValue& j, JsonPath p);  \
  virtual JsonValue toJson() const;

/** This header contains types which help form the Microsoft language server
    protocol. The types attempt to follow the specification as faithfully
    as possible, but liberty is taken in cases where definitions are too
    deeply nested or otherwise redundant.

    NOTE: If you define a new struct in this file, you should add a pair of
    (de)serializer functions to the section at the bottom of the file in
    order to define how the type is converted to/from JSON.
*/
namespace chpldef {

struct ProtocolStruct {
  virtual bool fromJson(const JsonValue& j, JsonPath p) = 0;
  virtual JsonValue toJson() const = 0;

  /** By default, convert to JSON and then print the JSON. */
  virtual std::string toString() const;
  virtual ~ProtocolStruct() = default;
};

/** Information about the client. */
struct ClientInfo : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();

  std::string name;
  opt<std::string> version;
};

/** TODO: Used to store 'chpldef' specific initialization options. */
struct ChpldefInit : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();
};

/** As defined by the spec, this structure is deeply nested and absolutely
    impossible to read/understand. I've opted to follow clangd's lead here
    and only list things we should (probably) care about.

    For right now, this struct is empty (because the spec allows all the
    fields to be nullable).

    TODO: If you add a field here, then adjust the (de)serializer methods.
*/
struct ClientCapabilities : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();
};

struct WorkspaceFolder : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();

  std::string uri;
  std::string name;
};

struct TraceLevel : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();

  Logger::Level level;
};

struct InitializeParams : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();

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

/** TODO: Build this up in conjunction with 'ClientCapabilities'. */
struct ServerCapabilities : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();
};

struct ServerInfo : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();

  std::string name;
  opt<std::string> version;
};

struct InitializeResult : ProtocolStruct {
  CHPLDEF_PROTOCOL_STRUCT_OVERRIDES();

  ServerCapabilities capabilities;
  opt<ServerInfo> serverInfo;
};

} // end namespace 'chpldef'

/** Teach the LLVM JSON library how to use the protocol structs. */
namespace llvm {
namespace json {
  bool fromJSON(const llvm::json::Value& j, chpldef::ProtocolStruct& x,
                llvm::json::Path p);
  llvm::json::Value toJSON(const chpldef::ProtocolStruct& x);
} // end namespace 'json'
} // end namespace 'llvm'

#endif
