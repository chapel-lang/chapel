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
#include "chpl/framework/ErrorBase.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/uast/AstNode.h"
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

  class ErrorHandler : public chpl::Context::ErrorHandler {
  public:
    using Error = chpl::ErrorBase;
    using ErrorAndRevision = std::pair<chpl::owned<Error>, int64_t>;
    using ErrorList = std::vector<ErrorAndRevision>;
  private:
    ErrorList errors_;
    Server* ctx_ = nullptr;
  public:
    ErrorHandler(Server* ctx) : ctx_(ctx) {}
   ~ErrorHandler() = default;
    virtual void report(chpl::Context* chapel, const Error* err) override;
    inline const ErrorList& errors() const { return errors_; }
    inline void clear() { errors_.clear(); }
    inline size_t numErrors() { return errors_.size(); }
  };

private:
  State state_ = UNINIT;
  Logger logger_;
  chpl::owned<chpl::Context> chapel_ = nullptr;
  Configuration config_;
  TextRegistry textRegistry_;
  int64_t revision_;
  ErrorHandler* errorHandler_ = nullptr;

  chpl::owned<chpl::Context> createCompilerContext();
  bool shouldGarbageCollect();
  bool shouldPrepareToGarbageCollect();

  inline bool isLogLevel(Logger::Level level) const {
    return logger_.level() == level;
  }

protected:
  friend class chpldef::Initialize;
  friend class chpldef::Initialized;
  friend class chpldef::Shutdown;
  friend class chpldef::DidOpen;

  inline void setState(State state) { state_ = state; }
  inline TextRegistry& textRegistry() { return textRegistry_; }

public:
  Server(Configuration config);
 ~Server() = default;

  inline ErrorHandler* errorHandler() { return errorHandler_; }
  inline State state() const { return state_; }
  inline int64_t revision() const { return revision_; }
  inline const chpl::Context* chapel() const { return chapel_.get(); }

  void setLogger(Logger&& logger);
  inline Logger& logger() { return logger_; }
  inline bool isLogMessage() const { return isLogLevel(Logger::MESSAGES); }
  inline bool isLogVerbose() const { return isLogLevel(Logger::VERBOSE); }
  inline bool isLogTrace() const { return isLogLevel(Logger::TRACE); }
  CHPLDEF_PFMT(2, 3, void message(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void verbose(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void trace(const char* fmt, ...));

  void sleep(int msec);

  enum WithChapelConfig {
    CHPL_NO_MASK         = 0,
    CHPL_BUMP_REVISION   = 1,
  };

  /** Execute code with controlled access to the Chapel context. */
  template <typename F, typename ...Ns>
  auto withChapel(WithChapelConfig c, F&& f, Ns... ns)
  -> decltype(f(chapel_.get(), ns...)) {
    if (shouldGarbageCollect()) chapel_->collectGarbage();
    if (c & CHPL_BUMP_REVISION) {
      chapel_->advanceToNextRevision(shouldPrepareToGarbageCollect());
      ++revision_;
    }
    return f(chapel_.get(), ns...);
  }

  /** Execute code with controlled access to the Chapel context. */
  template <typename F, typename ...Ns>
  auto withChapel(F&& f, Ns... ns)
  -> decltype(withChapel(CHPL_NO_MASK, f, ns...)) {
    return withChapel(CHPL_NO_MASK, f, ns...);
  }

  enum FormatDetail {
    DEFAULT = 0
  };

private:
  void fmtImpl(std::stringstream& ss, FormatDetail dt,
               const chpl::uast::AstNode* t);
  void fmtImpl(std::stringstream& ss, FormatDetail dt,
               const chpl::Location& t);
public:
  template <typename T>
  std::string fmt(const T& t, FormatDetail dt=FormatDetail::DEFAULT) {
    std::stringstream ss;
    fmtImpl(ss, dt, t);
    return ss.str();
  }
};

} // end namespace 'chpldef'

#endif
