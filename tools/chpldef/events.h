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

#ifndef CHPL_TOOLS_CHPLDEF_EVENTS_H
#define CHPL_TOOLS_CHPLDEF_EVENTS_H

#include "Server.h"

namespace chpldef {
namespace events {

/** This event reads a message from the transport layer. */
class ReadMessage : public Server::Event {
  static constexpr auto NAME = "ReadMessage";
  static constexpr auto WHEN = LOOP_START;
public:
  ReadMessage() : Event(NAME, WHEN) {}
  virtual ~ReadMessage() = default;
  virtual bool canRun(Server* ctx) const override;
  virtual void run(Server* ctx, const Message* msg, When when) override;
};

/** This event ensures modules are resolved. */
class ResolveModules : public Server::Event {
  static constexpr auto NAME = "ResolveModules";
  static constexpr auto WHEN = AFTER_HANDLE;
  bool resolveModulesAfterHandle_ = false;
  int64_t lastRevisionResolved_ = -1;
public:
  ResolveModules() : Event(NAME, WHEN) {}
  virtual ~ResolveModules() = default;
  virtual void run(Server* ctx, const Message* msg, When when) override;
};

/** This event may prepare a 'PublishDiagnostics' message to send. */
class PreparePublishDiagnostics : public Server::Event {
  static constexpr auto NAME = "PreparePublishDiagnostics";
  static constexpr auto WHEN = PRIOR_HANDLE | AFTER_HANDLE;
public:
  PreparePublishDiagnostics() : Event(NAME, WHEN) {}
  virtual ~PreparePublishDiagnostics() = default;
  virtual void run(Server* ctx, const Message* msg, When when) override;
};

} // end namespace 'events'
} // end namespace 'chpldef'

#endif
