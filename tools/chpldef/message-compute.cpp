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

/** This is one file where message handlers can be implemented. However, if
    a particular message's handler grows to be very large in size (e.g.,
    perhaps 500+ lines), then we should feel free to move it to a different
    source file with a name that matches the message name. */
namespace chpldef {

static void
doConfigureStaticCapabilities(Server* ctx, ServerCapabilities& x) {
  x.positionEncoding = "utf-16";
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
  ret.name = "chpldef";
  ret.version = "0.0.0";
  return ret;
}

Initialize::ComputedResult Initialize::compute(Server* ctx) {

  // Set the log verbosity level if it was requested.
  if (auto trace = p.trace) {
    auto level = trace->level;
    ctx->message("Client requested log level: '%s'\n",
                 Logger::levelToString(level));
    auto &logger = ctx->logger();
    if (level < logger.level()) {
      ctx->message("Ignoring as level is '%s'\n", logger.levelToString());
    } else {
      ctx->logger().setLevel(level);
    }
  }

  Result ret;

  doConfigureStaticCapabilities(ctx, ret.capabilities);
  ret.serverInfo = configureServerInfo(ctx);

  return ret;
}

} // end namespace 'chpldef'
