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

#include "./Message.h"
#include "./Server.h"
#include "chpl/parsing/parsing-queries.h"

namespace {
  static constexpr bool UNSUPPORTED_DELIBERATELY = false;
  static constexpr bool UNSUPPORTED_TODO = false;
  static constexpr bool SUPPORTED = true;
}

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

template <>
Initialize::ComputeResult
Initialize::compute(Server* ctx, ComputeParams p) {

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
  CHPL_ASSERT(ctx->state() == Server::UNINITIALIZED);
  ctx->setState(Server::SETUP);

  Result ret;

  doConfigureStaticCapabilities(ctx, ret.capabilities);

  ret.serverInfo = configureServerInfo(ctx);

  return ret;
}

template <>
Initialized::ComputeResult
Initialized::compute(Server* ctx, ComputeParams p) {
  CHPL_ASSERT(ctx->state() == Server::SETUP);
  ctx->setState(Server::READY);
  return {};
}

template <>
Shutdown::ComputeResult
Shutdown::compute(Server* ctx, ComputeParams p) {
  CHPL_ASSERT(ctx->state() == Server::READY);
  ctx->setState(Server::SHUTDOWN);
  return {};
}

template <>
Exit::ComputeResult
Exit::compute(Server* ctx, ComputeParams p) {
  CHPL_ASSERT(ctx->state() == Server::SHUTDOWN);
  return {};
}

} // end namespace 'chpldef'
