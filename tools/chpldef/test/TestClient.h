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

#ifndef CHPL_TOOLS_CHPLDEF_TESTCLIENT_H
#define CHPL_TOOLS_CHPLDEF_TESTCLIENT_H

#include "compiler-gadgets.h"
#include "events.h"
#include "Format.h"
#include "Logger.h"
#include "Message.h"
#include "misc.h"
#include "protocol-types.h"
#include "Server.h"
#include "Transport.h"
#include <ostream>

using namespace chpldef;

/** This 'TestClient' simulates a LSP client, but issues direct calls to
    server functionality rather than by sending JSON over some sort of
    transport layer as a normal client would (also, usually the client
    operates as a separate process - here for the sake of unit testing
    the client and server operate in the same process). This client should
    be used to test most language server queries.
*/
class TestClient {
  Server server_;
  Server* ctx_ = &server_;
  int64_t messageIdCounter_ = 0;
  std::map<std::string, int64_t> uriToVersion_;
  std::ostream& dbg_ = ctx_->transport() ? std::cerr : std::cout;

  static Server createServerInstance();
  static chpl::Context createChapelCompilerInstance();
  int64_t bumpVersionForUri(const std::string& uri);

public:
  TestClient() : server_(createServerInstance()), ctx_(&server_) {}
 ~TestClient() = default;

  /** Get a handle to a stream suitable for printing debug infos. */
  inline std::ostream& dbg() { return dbg_; }

  /** For requests, prepare a new ID number embedded in JSON. */
  JsonValue createUniqueMessageId();

  /** Send the 'Initialize' message to the server. */
  opt<InitializeResult> sendInitialize();

  /** Send the 'Initialized' message to the server. */
  void sendInitialized();

  /** Send the 'Shutdown' message to the server. */
  void sendShutdown();

  /** Send the 'Exit' message to the server. */
  void sendExit();

  /** Complete the initialization handshake. After this call the server will
      be in the 'READY' state and can accept any message. */
  void advanceServerToReady();

  /** Send a 'DidOpen' notification. */
  void sendDidOpen(const std::string& uri, const std::string& text);

  /** A mention represents a text range the client is interested in.
      Mentions cannot actually store AST because they were computed
      with a separate instance of the Chapel compiler that has a short
      lifetime. */
  struct Mention {
    using AstTag = chpl::uast::asttags::AstTag;
    AstTag tag;
    bool isCallBaseExpression = false;
    std::string symbol;
    Location source;        /** This is a LSP location, not Chapel's! */
    Location target;        /** Which means it is 0-based! */
    bool isValid = true;    /** If there is no target, this is false. */

    std::string toString() const;
  };

  /** Collect mention the client is interested in using a separate instance
      of the Chapel compiler. The way mentions are mapped to target
      locations depends on the target. Calls are mapped linearly (where call
      N of 'foo' maps to overload N of 'foo') in order to simulate
      overloading. Every other declaration is mapped by name.
  */
  static std::vector<Mention>
  collectMentions(const std::string& uri, const std::string& text);

  /** Collect line lengths within the file using a separate instance
      of the Chapel compiler. */
  static std::vector<uint64_t>
  collectLineLengthsInSource(const std::string& text);

  /** Send a 'Declaration' message. May return a 'Location'. */
  opt<Location> sendDeclaration(const std::string& uri, Position cursor);

  /** For testing/debugging purposes. */
  static void breakpoint();
};

#endif
