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

class Server {
public:
  enum State {
    UNINIT,             /** Client has not sent 'Initialize' yet. */
    INIT,               /** We have responded to 'Initialize'. */
    READY,              /** Client has sent us 'Initialized'. */
    SHUTDOWN            /** Client has sent us 'Shutdown'. */
  };

  class Configuration {};

private:
  State state_ = UNINIT;
  Logger logger_;
  chpl::owned<chpl::Context> chplctx_ = nullptr;
  int revision_;

protected:
  friend class chpldef::Shutdown;
  friend class chpldef::Initialize;
  friend class chpldef::Initialized;

  inline void setState(State state) { state_ = state; }

public:
  Server();
 ~Server() = default;

  inline State state() const { return state_; }
  inline int revision() const { return revision_; }
  inline const chpl::Context* chplctx() const { return chplctx_.get(); }

  void setLogger(Logger&& logger);
  inline Logger& logger() { return logger_; }
  void sleep(int msec);

  CHPLDEF_PFMT(2, 3, void message(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void verbose(const char* fmt, ...));
  CHPLDEF_PFMT(2, 3, void trace(const char* fmt, ...));
};

} // end namespace 'chpldef'

#endif
