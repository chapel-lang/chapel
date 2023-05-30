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

#include "./command-line-flags.h"
#include "./Message.h"
#include "./Server.h"
#include "./Transport.h"
#include <iostream>

using namespace chpldef;

static Server::Configuration prepareServerConfig(int argc, char** argv) {
  Server::Configuration ret;

  cmd::doParseOptions(argc, argv);

  if (!cmd::chplHome.empty()) {
    ret.chplHome = cmd::chplHome;
  } else if (const char* chplHomeEnv = getenv("CHPL_HOME")) {
    ret.chplHome = chplHomeEnv;
  } else {
    std::cerr << "No value for '$CHPL_HOME'!" << std::endl;
  }

  // TODO: Wire these up to command-line flags.
  ret.logFile = cmd::logFile;
  ret.logLevel = cmd::logLevel;
  ret.garbageCollectionFrequency = 0;
  ret.warnUnstable = false;
  ret.enableStandardLibrary = false;
  ret.compilerDebugTrace = false;

  return ret;
}

int main(int argc, char** argv) {
  auto config = prepareServerConfig(argc, argv);
  Server context(std::move(config));
  Server* ctx = &context;

  // Flush every log message immediately to avoid losing info on crash.
  auto& log = ctx->logger();
  log.setFlushImmediately(true);

  ctx->message("Logging to '%s' with level '%s'\n",
               log.filePath().c_str(),
               log.levelToString());

  int run = 1;
  int ret = 0;

  while (run) {
    ctx->message("Server awaiting message...\n");

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
    bool ok = Transport::readJsonBlocking(ctx, std::cin, json);
    CHPL_ASSERT(ok);

    if (log.level() == Logger::TRACE) {
      ctx->trace("Incoming JSON is %s\n", jsonToString(json).c_str());
    }

    // Create a message from the incoming JSON...
    auto msg = Message::request(ctx, std::move(json));
    CHPL_ASSERT(msg.get());

    // Parsing the JSON could have failed.
    if (msg->status() == Message::FAILED || msg->isResponse()) {
      CHPLDEF_TODO();
    }

    CHPL_ASSERT(msg->status() == Message::PENDING);
    CHPL_ASSERT(!msg->isOutbound());

    if (msg->tag() == Message::Exit) {
      CHPL_ASSERT(ctx->state() == Server::SHUTDOWN);
      run = 0;
    }

    // We have an immediate response, so send it.
    if (auto optRsp = Message::handle(ctx, msg.get())) {
      auto pack = optRsp->pack();
      if (log.level() == Logger::TRACE) {
        auto str = jsonToString(pack);
        ctx->trace("Outgoing JSON is %s\n", str.c_str());
      }

      ok = Transport::sendJsonBlocking(ctx, std::cout, std::move(pack));
      CHPL_ASSERT(ok);

    // Response was delayed.
    } else {
      if (!msg->isNotification()) {
        CHPLDEF_FATAL(ctx, "Handler response should not be delayed!");
      }
    }

    // Flush the log in case something goes wrong.
    log.flush();
  }

  ctx->message("Server exiting with code '%d'\n", ret);
  log.flush();

  return ret;
}
