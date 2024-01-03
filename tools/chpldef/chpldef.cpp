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

  ret.logFile = cmd::logFile;
  ret.logLevel = cmd::logLevel;
  ret.garbageCollectionFrequency = cmd::garbageCollectionFrequency;
  ret.warnUnstable = cmd::warnUnstable;
  ret.enableStandardLibrary = cmd::enableStandardLibrary;
  ret.compilerDebugTrace = cmd::compilerDebugTrace;

  // TODO: Configure transport with a flag.
  ret.transport = chpl::toOwned(new TransportStdio());

  return ret;
}

int main(int argc, char** argv) {
  auto config = prepareServerConfig(argc, argv);
  Server server(std::move(config));
  Server* ctx = &server;

  // Flush every log message immediately to avoid losing info on crash.
  auto& log = ctx->logger();
  log.setFlushImmediately(true);

  ctx->message("Logging to '%s' with level '%s'\n",
               log.filePath().c_str(),
               log.levelToString());

  int ret = ctx->run();

  ctx->message("Server exiting with code '%d'\n", ret);
  log.flush();

  return ret;
}
