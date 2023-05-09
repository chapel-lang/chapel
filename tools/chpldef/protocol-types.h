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

namespace chpldef {

/** The progress token is either an 'integer' or a 'string'. */
struct WorkDoneProgressParams {
  JsonValue token = nullptr;
};

struct ClientInfo {
  std::string name;
  opt<std::string> version;
};

/** As defined by the spec, this structure is deeply nested and absolutely
    impossible to read/understand. I've opted to follow clangd's lead here
    and only list things we should (probably) care about.
    For now this thing is empty, but as we find we need to care about
    certain client capabilities, we'll add them here.
*/
struct ClientCapabilities {
  opt<int> dummy;
};

struct WorkspaceFolder {
  opt<int> dummy;
};

struct InitializeParams : WorkDoneProgressParams {
  opt<int> proccessId;
  opt<ClientInfo> clientInfo;
  opt<std::string> locale;
  opt<std::string> rootPath;  /** Deprecated - see 'rootUri'. */
  opt<std::string> rootUri;   /** Deprecated - see 'workspaceFolders'. */
  JsonValue initializationOptions = nullptr;
  ClientCapabilities clientCapabilities;
  Logger::Level trace;
  opt<std::vector<WorkspaceFolder>> workspaceFolders;
};

bool fromJson(InitializeParams& data, const JsonValue& val, JsonPath p);
JsonValue toJson(const InitializeParams& data);

struct InitializeResult {
  opt<int> dummy;
};

bool fromJson(InitializeResult& data, const JsonValue& val, JsonPath p);
JsonValue toJson(const InitializeResult& data);

struct ServerCapabilities {
  opt<int> dummy;
};




} // end namespace 'chpldef'

#endif
