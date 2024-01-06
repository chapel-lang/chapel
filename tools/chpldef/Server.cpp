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

#include "Server.h"
#include "Message.h"
#include "events.h"
#include "Transport.h"
#include "chpl/framework/Context.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/util/filesystem.h"
#include "chpl/util/printf.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <chrono>
#include <thread>

// Wrapper to forward printf arguments to logger.
#define VPRINTF_FORWARD_(fmt__, func__) \
  do { \
    va_list args__; \
    va_start(args__, fmt__); \
    func__(fmt__, args__); \
    va_end(args__); \
  } while (0)

namespace chpldef {

void Server::ErrorHandler::report(chpl::Context* chapel,
                                  const chpl::ErrorBase* err) {
  auto p = std::make_pair(err->clone(), ctx_->revision());
  errors_.push_back(std::move(p));
}

void Server::setLogger(Logger&& logger) {
  this->logger_ = std::move(logger);
}

void Server::sleep(int msec) {
  CHPL_ASSERT(msec >= 0);
  std::this_thread::sleep_for(std::chrono::milliseconds(msec));
}

void Server::message(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, logger_.vmessage);
}

void Server::verbose(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, logger_.vverbose);
}

void Server::trace(const char* fmt, ...) {
  VPRINTF_FORWARD_(fmt, logger_.vtrace);
}

bool Server::Event::canRun(Server* ctx) const {
  return ctx->state() == Server::READY;
}

Server::Server(Server::Configuration config)
    : chapel_(createCompilerContext(config)),
      config_(std::move(config)) {

  // Install the server error handler.
  auto handler = toOwned(new ErrorHandler(this));
  errorHandler_ = handler.get();
  chapel_.installErrorHandler(std::move(handler));

  // Open the server log.
  Logger logger;
  if (!config_.logFile.empty()) {
    logger = Logger::createForFile(config_.logFile);
    if (!logger.isLogging()) {
      std::cerr << "Failed to open log file!" << std::endl;
      logger = Logger();
      std::cerr << "Using '" << logger.filePath() << "'" << std::endl;
    }
  }

  // Configure and install the logger.
  logger.setHeader("[chpldef]");
  logger.setLevel(config_.logLevel);
  this->setLogger(std::move(logger));

  // Set a quick alias for the transport.
  this->transport_ = config_.transport.get();

  doRegisterEssentialEvents();
}

chpl::Context
Server::createCompilerContext(const Server::Configuration& config) const {
  chpl::Context::Configuration chplConfig;
  chplConfig.chplHome = config.chplHome;
  return chpl::Context(std::move(chplConfig));
}

bool Server::shouldGarbageCollect() const {
  auto f = config_.garbageCollectionFrequency;
  if (f == 1) return true;
  if (f == 0) return false;
  return (revision_ % f) == 0;
}

bool Server::shouldPrepareToGarbageCollect() const {
  auto f = config_.garbageCollectionFrequency;
  if (f == 0) return false;
  return (revision_ % f) == (f - 1);
}

void Server::doRegisterEssentialEvents() {
  using namespace events;
  registerEvent(chpl::toOwned(new ReadMessage()));
  registerEvent(chpl::toOwned(new ResolveModules()));
  registerEvent(chpl::toOwned(new PreparePublishDiagnostics()));
}

void Server::doRunEvents(Event::When when, const Message* msg) {
  for (auto& e : events_) {
    CHPL_ASSERT(e.get());
    const bool run = (e->mask() & when) || (e->mask() & Event::ALWAYS);
    if (!run || !e->canRun(this)) continue;
    this->trace("Running event '%s'\n", e->name());
    e->run(this, msg, when);
  }
}

chpl::owned<Message> Server::dequeueOneMessage() {
  if (messages_.empty()) return nullptr;
  auto ret = std::move(messages_.front());
  messages_.pop();
  return ret;
}

void Server::sendMessage(const Message* msg) {
  if (transport_ == nullptr) return;

  if (auto j = msg->pack()) {
    if (isLogTrace()) {
      auto str = jsonToString(*j);
      this->trace("Outgoing JSON is: %s\n", str.c_str());
    }

    auto status = transport_->sendJson(this, *j);
    if (status != Transport::OK) {
      CHPLDEF_FATAL(this, "Failed to send JSON!");
    }
  }
}

void Server::registerEvent(chpl::owned<Event> event) {
  if (!event.get()) return;
  events_.push_back(std::move(event));
}

void Server::enqueue(chpl::owned<Message> msg) {
  if (!msg.get()) return;
  messages_.push(std::move(msg));
}

bool Server::handle(chpl::owned<Message> msg) {
  if (!msg.get()) return false;
  bool ret = false;

  // Handle a response to an outbound request.
  if (auto rsp = msg->toResponse()) {
    CHPL_ASSERT(rsp->isCompleted());
    CHPL_ASSERT(msg->id().kind() != JsonValue::Null);

    auto it = idToOutboundRequest_.find(msg->idToString());
    if (it != idToOutboundRequest_.end()) {
      auto& req = it->second;
      CHPL_ASSERT(req->behavior() == Message::OUTBOUND_REQUEST);
      req->handle(this, rsp);
      idToOutboundRequest_.erase(it);
    } else {
      CHPLDEF_TODO();
    }

    return false;
  }

  if (msg->status() == Message::PROGRESSING) CHPLDEF_TODO();

  switch (msg->behavior()) {
    case Message::INCOMING_REQUEST:
    case Message::INCOMING_NOTIFY: {
      if (msg->status() == Message::FAILED) CHPLDEF_TODO();
      msg->handle(this);
      if (msg->status() == Message::FAILED) CHPLDEF_TODO();
      if (!msg->isNotification()) sendMessage(msg.get());
    } break;

    case Message::OUTBOUND_NOTIFY:
    case Message::OUTBOUND_REQUEST: {
      if (msg->status() == Message::FAILED) CHPLDEF_TODO();
      CHPL_ASSERT(msg->status() == Message::PENDING);
      sendMessage(msg.get());

      // Requests are stored for later evaluation.
      if (!msg->isNotification()) {
        auto idStr = msg->idToString();
        auto it = idToOutboundRequest_.find(idStr);
        if (it != idToOutboundRequest_.end()) CHPLDEF_TODO();
        auto p = std::make_pair(std::move(idStr), std::move(msg));
        idToOutboundRequest_.emplace(std::move(p));
      }
    } break;

    default: {
      CHPLDEF_IMPOSSIBLE();
    } break;
  }

  // Prepare to exit if we received the 'Exit' message.
  if (msg->tag() == MessageTag::Exit) {
    CHPL_ASSERT(state() == Server::SHUTDOWN);

    // Should not have any messages or pending requests in the queue.
    if (!messages_.empty()) CHPLDEF_TODO();
    if (!idToOutboundRequest_.size()) CHPLDEF_TODO();
    ret = true;
  }

  return ret;
}

int Server::run() {
  bool running = true;
  int ret = 0;

  while (running) {
    doRunEvents(Event::LOOP_START, nullptr);

    if (auto msg = dequeueOneMessage()) {
      doRunEvents(Event::PRIOR_HANDLE, msg.get());
      const bool isTimeToExit = handle(std::move(msg));
      running = !isTimeToExit;
      doRunEvents(Event::AFTER_HANDLE, msg.get());
    }

    logger().flush();
  }

  return ret;
}

} // end namespace 'chpldef'
