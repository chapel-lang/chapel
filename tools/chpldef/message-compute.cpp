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

/** This is one file where message handlers can be implemented. However, if
    a particular message's handler grows to be very large in size (e.g.,
    perhaps 500+ lines), then we should feel free to move it to a different
    source file with a name that matches the message name. */
namespace chpldef {

static TextDocumentSyncOptions
configureTextDocumentSyncOptions(Server* ctx) {
  TextDocumentSyncOptions ret;
  ret.openClose = true;
  ret.change = TextDocumentSyncOptions::Full;   /** TODO: Incremental? */
  ret.willSave = true;
  ret.willSaveWaitUntil = true;
  ret.save = SaveOptions();
  ret.save->includeText = true;
  return ret;
}

static void
doConfigureStaticCapabilities(Server* ctx, ServerCapabilities& x) {
  x.positionEncoding = "utf-16";
  x.textDocumentSync = configureTextDocumentSyncOptions(ctx);
  x.hoverProvider = false;
  x.declarationProvider = false;
  x.definitionProvider = false;
  x.typeDefinitionProvider = false;
  x.implementationProvider = false;
  x.referencesProvider = false;
  x.documentHighlightProvider = false;
  x.documentSymbolProvider = false;
  x.codeActionProvider = false;
  x.colorProvider = false;
  x.documentFormattingProvider = false;
  x.documentRangeFormattingProvider = false;
  x.renameProvider = false;
  x.foldingRangeProvider = false;
  x.selectionRangeProvider = false;
  x.linkEditingRangeProvider = false;
  x.callHierarchyProvider = false;
  x.monikerProvider = false;
  x.typeHierarchyProvider = false;
  x.inlineValueProvider = false;
  x.inlayHintProvider = false;
  x.workspaceSymbolProvider = false;
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

DidOpen::ComputedResult
DidOpen::compute(Server* ctx, const Params& p) {
  auto& tdi = p.textDocument;
  auto& e = ctx->textRegistry()[tdi.uri];

  if (e.isOpen) {
    CHPLDEF_TODO();
    return fail();
  }

  CHPL_ASSERT(tdi.version > e.version);

  // NOTE: I think we always have to bump the revision here. This is
  // because this file may have been implicitly parsed from disk as
  // as result of resolving a use/import. The contents are considered
  // to have changed and the "truth of the file's contents" are determined
  // by the client as long as it has the file open. Cannot implicitly
  // read from disk, so have to bump the revision to ensure correctness.
  ctx->withChapelContext(Server::CHPL_BUMP_REVISION,
  [&](auto chapel) {
    chpl::parsing::setFileText(chapel, tdi.uri, tdi.text);
    auto& fc = chpl::parsing::fileText(chapel, tdi.uri);
    CHPL_ASSERT(!fc.error());
    CHPL_ASSERT(fc.text() == tdi.text);
    e.version = tdi.version;
    e.lastRevisionContentsUpdated = ctx->revision();
    e.isOpen = true;
  });

  return {};
}

DidChange::ComputedResult
DidChange::compute(Server* ctx, const Params& p) {
  CHPLDEF_TODO();
  return {};
}

DidSave::ComputedResult
DidSave::compute(Server* ctx, const Params& p) {
  CHPLDEF_TODO();
  return {};
}

DidClose::ComputedResult
DidClose::compute(Server* ctx, const Params& p) {
  CHPLDEF_TODO();
  return {};
}

} // end namespace 'chpldef'
