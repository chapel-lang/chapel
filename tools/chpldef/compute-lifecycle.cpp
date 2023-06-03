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

#include "./Message.h"
#include "./Server.h"
#include "chpl/parsing/parsing-queries.h"

namespace {
  static constexpr bool UNSUPPORTED_DELIBERATELY = false;
  static constexpr bool UNSUPPORTED_TODO = false;
  static constexpr bool SUPPORTED = true;
}

/** This is one file where message handlers can be implemented. However, if
    a particular message's handler grows to be very large in size (e.g.,
    perhaps 500+ lines), then we should feel free to move it to a different
    source file with a name that matches the message name. */
namespace chpldef {

static TextDocumentSyncOptions
configureTextDocumentSync(Server* ctx) {
  TextDocumentSyncOptions ret;
  ret.openClose         = SUPPORTED;
  ret.change            = TextDocumentSyncOptions::Full;
  ret.willSave          = SUPPORTED;
  ret.willSaveWaitUntil = SUPPORTED;
  ret.save              = SaveOptions();
  ret.save->includeText = true;
  return ret;
}

static void
doConfigureStaticCapabilities(Server* ctx, ServerCapabilities& x) {
  x.positionEncoding                  = "utf-16";
  x.textDocumentSync                  = configureTextDocumentSync(ctx);
  x.hoverProvider                     = UNSUPPORTED_TODO;
  x.declarationProvider               = SUPPORTED;
  x.definitionProvider                = UNSUPPORTED_DELIBERATELY;
  x.typeDefinitionProvider            = UNSUPPORTED_TODO;
  x.implementationProvider            = UNSUPPORTED_TODO;
  x.referencesProvider                = UNSUPPORTED_TODO;
  x.documentHighlightProvider         = UNSUPPORTED_TODO;
  x.documentSymbolProvider            = UNSUPPORTED_TODO;
  x.codeActionProvider                = UNSUPPORTED_TODO;
  x.colorProvider                     = UNSUPPORTED_TODO;
  x.documentFormattingProvider        = UNSUPPORTED_TODO;
  x.documentRangeFormattingProvider   = UNSUPPORTED_TODO;
  x.renameProvider                    = UNSUPPORTED_TODO;
  x.foldingRangeProvider              = UNSUPPORTED_TODO;
  x.selectionRangeProvider            = UNSUPPORTED_TODO;
  x.linkEditingRangeProvider          = UNSUPPORTED_TODO;
  x.callHierarchyProvider             = UNSUPPORTED_TODO;
  x.monikerProvider                   = UNSUPPORTED_TODO;
  x.typeHierarchyProvider             = UNSUPPORTED_TODO;
  x.inlineValueProvider               = UNSUPPORTED_TODO;
  x.inlayHintProvider                 = UNSUPPORTED_TODO;
  x.workspaceSymbolProvider           = UNSUPPORTED_TODO;
}

static ServerInfo configureServerInfo(Server* ctx) {
  ServerInfo ret;
  ret.name = Server::NAME;
  ret.version = Server::VERSION;
  return ret;
}

Initialize::ComputedResult
Initialize::compute(Server* ctx, const Params& p) {

  // Set the log verbosity level if it was requested.
  if (auto trace = p.trace) {
    auto level = trace->level;
    ctx->message("Client requested log level '%s'\n",
                 Logger::levelToString(level));
    auto &logger = ctx->logger();
    if (level < logger.level()) {
      ctx->message("Ignoring as level is '%s'\n", logger.levelToString());
    } else {
      ctx->logger().setLevel(level);
    }
  }

  // Set the server to the 'INIT' state.
  CHPL_ASSERT(ctx->state() == Server::UNINIT);
  ctx->setState(Server::INIT);

  Result ret;

  doConfigureStaticCapabilities(ctx, ret.capabilities);

  ret.serverInfo = configureServerInfo(ctx);

  return ret;
}

Initialized::ComputedResult
Initialized::compute(Server* ctx, const Params& p) {
  CHPL_ASSERT(ctx->state() == Server::INIT);
  ctx->setState(Server::READY);
  return {};
}

Shutdown::ComputedResult
Shutdown::compute(Server* ctx, const Params& p) {
  CHPL_ASSERT(ctx->state() == Server::READY);
  ctx->setState(Server::SHUTDOWN);
  return {};
}

Exit::ComputedResult
Exit::compute(Server* ctx, const Params& p) {
  CHPL_ASSERT(ctx->state() == Server::SHUTDOWN);
  return {};
}

} // end namespace 'chpldef'
