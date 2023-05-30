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

#ifndef CHPL_TOOLS_CHPLDEF_SERVER_H
#define CHPL_TOOLS_CHPLDEF_SERVER_H

#include "./Logger.h"
#include "./misc.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "llvm/ADT/Optional.h"
#include "llvm/Support/JSON.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

namespace chpldef {

class Initialize;
class Initialized;
class Shutdown;
class DidOpen;

class Server {
public:
  static constexpr const char* NAME = "chpldef";
  static constexpr const char* VERSION = "0.0.0";
  static constexpr int DEFAULT_GC_FREQUENCY = 64;

  enum State {
    UNINIT,             /** Client has not sent 'Initialize' yet. */
    INIT,               /** We have responded to 'Initialize'. */
    READY,              /** Client has sent us 'Initialized'. */
    SHUTDOWN            /** Client has sent us 'Shutdown'. */
  };

  // TODO: Use this to configure server capabilities, etc.
  struct Configuration {
    std::string logFile;
    Logger::Level logLevel = Logger::OFF;
    std::string chplHome;
    int garbageCollectionFrequency = DEFAULT_GC_FREQUENCY;
    bool warnUnstable = false;
    bool enableStandardLibrary = false;
    bool compilerDebugTrace = false;
  };

  struct TextEntry {
    int64_t version = -1;
    int64_t lastRevisionContentsUpdated = -1;
    bool isOpen = false;
  };

  using TextRegistry = std::map<std::string, TextEntry>;

private:
  State state_ = UNINIT;
  Logger logger_;
  chpl::owned<chpl::Context> chapel_ = nullptr;
  Configuration config_;
  TextRegistry textRegistry_;
  int64_t revision_;

  chpl::owned<chpl::Context> createCompilerContext();
  bool shouldGarbageCollect();
  bool shouldPrepareToGarbageCollect();

protected:
  friend class chpldef::Initialize;
  friend class chpldef::Initialized;
  friend class chpldef::Shutdown;
  friend class chpldef::DidOpen;

  inline void setState(State state) { state_ = state; }
  inline TextRegistry& textRegistry() { return textRegistry_; };

public:
  Server(Configuration config);
 ~Server() = default;

  inline State state() const { return state_; }
  inline int64_t revision() const { return revision_; }
  inline const chpl::Context* chapel() const { return chapel_.get(); }

  void setLogger(Logger&& logger);
  inline Logger& logger() { return logger_; }
  void sleep(int msec);

  CHPLDEF_PFMT(2, 3, void message(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void verbose(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void trace(const char* fmt, ...));

  enum WithChapelContextConfig {
    CHPL_NO_MASK         = 0,
    CHPL_BUMP_REVISION   = 1,
  };

  /** Execute code with controlled access to the Chapel context. */
  template <typename T=void, typename F, typename ...Ns>
  T withChapelContext(F& f, Ns... ns) {
    return withChapelContext(CHPL_NO_MASK, f, ns...);
  }

  /** Execute code with controlled access to the Chapel context. */
  template <typename T=void, typename F, typename ...Ns>
  T withChapelContext(WithChapelContextConfig c, F&& f, Ns... ns) {
    if (shouldGarbageCollect()) chapel_->collectGarbage();
    if (c & CHPL_BUMP_REVISION) {
      chapel_->advanceToNextRevision(shouldPrepareToGarbageCollect());
      ++revision_;
    }
    return f(chapel_.get(), ns...);
  }
};

} // end namespace 'chpldef'

#endif
