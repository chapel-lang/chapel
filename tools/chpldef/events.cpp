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

#include "compiler-gadgets.h"
#include "events.h"
#include "Server.h"
#include "Transport.h"

namespace chpldef {
namespace events {

/** Always run this - we need to be reading messages. */
bool ReadMessage::canRun(Server* ctx) const { return true; }

void ReadMessage::run(Server* ctx, const Message* msg, When when) {
  CHPL_ASSERT(!msg && when == Event::LOOP_START);

  ctx->message("Attempting to read message...\n");
  auto ts = ctx->transport();

  if (ts == nullptr) {
    ctx->message("Giving up because no transport was found!\n");
    return;
  }

  JsonValue json(nullptr);

  // TODO: This operation blocks. We'd like non-blocking IO. There are a
  // few ways to accomplish this. Ideally we find some sort of high-level,
  // non-blocking "transport" abstraction that we can use.
  //
  //    -- LLVM might offer transport layer APIs
  //    -- LLVM might offer non-blocking IO
  //    -- C++ futures might be useful here, but we need to be able to
  //       control which thread runs the future and its lifetime (to
  //       make sure that the thread reading e.g., stdin is reliably
  //       closed...).
  //    -- We can do it ourselves using C++ threads, CVs, and locks, and
  //       wrap it up in a neat little function that populates a queue
  //       of messages for the context.
  auto status = ts->readJson(ctx, json);
  CHPL_ASSERT(status == Transport::OK);

  if (ctx->isLogTrace()) {
    ctx->trace("Incoming JSON is %s\n", jsonToString(json).c_str());
  }

  // Create a message from the incoming JSON...
  auto work = Message::create(ctx, std::move(json));
  CHPL_ASSERT(work.get());

  // Add the message to the work queue.
  ctx->enqueue(std::move(work));
}

static bool resolveModulesForMessageTag(MessageTag tag) {
  return tag == MessageTag::DidOpen;
}

void ResolveModules::run(Server* ctx, const Message* msg, When when) {
  if (when == Event::PRIOR_HANDLE) {
    CHPL_ASSERT(msg);
    resolveModulesAfterHandle_ = resolveModulesForMessageTag(msg->tag());
    return;
  }

  CHPL_ASSERT(!msg && when == AFTER_HANDLE);

  if (!resolveModulesAfterHandle_) return;
  if (lastRevisionResolved_ == ctx->revision()) return;

  // Commit to resolving.
  lastRevisionResolved_ = ctx->revision();

  for (const auto& p : ctx->textRegistry()) {
    const auto& uri = p.first;
    const auto& entry = p.second;
    if (!entry.isOpen) continue;
    ctx->withChapel([&](auto chapel) {
      auto& br = parseFromUri(chapel, uri);
      for (auto ast : br.topLevelExpressions()) {
        if (!ast->isModule()) continue;
        auto& rr = chpl::resolution::resolveModule(chapel, ast->id());
        std::ignore = rr;
      }
    });
  }
}

void PreparePublishDiagnostics::run(Server* ctx, const Message* msg,
                                    When when) {
  std::ignore = ctx;
}

} // end namespace 'events'
} // end namespace 'chpldef'
