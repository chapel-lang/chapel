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
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <iostream>
#include <fstream>

namespace chpldef {

bool Message::isIdValid(const JsonValue& id) {
  return id.kind() == JsonValue::Number || id.kind() == JsonValue::String;
}

// TODO: Need to build an error message or record what went wrong. Create
// an 'Invalid' subclass and use that to record any error that occurs past
// the point of parsing the message ID.
chpl::owned<Message> Message::request(Server* ctx, const JsonValue& json) {
  auto objPtr = json.getAsObject();
  if (!objPtr) {
    ctx->verbose("Failed to unpack JSON object, found: %s\n",
                 jsonTagStr(json));
    return nullptr;
  }

  // Get the ID as a JSON value.
  JsonValue id = nullptr;
  if (auto idPtr = objPtr->get("id")) {
    if (Message::isIdValid(*idPtr)) id = *idPtr;
  }

  // Failed to get the ID.
  if (id.kind() == JsonValue::Null) {
    ctx->verbose("Failed to get message ID\n");
    return nullptr;
  }

  // Determine what method is to be called.
  Message::Tag tag = Message::UNSET;
  if (auto optMethod = objPtr->getString("method")) {
    tag = Message::jsonRpcMethodNameToTag(optMethod->str());

    // TODO: We can create an 'Invalid' request to represent this failure.
    bool hasTag = tag != Message::UNSET && tag != Message::INVALID;
    if (!hasTag) {
      CHPLDEF_TODO();
      return nullptr;
    }
  }

  CHPL_ASSERT(tag != Message::RESPONSE);

  // Before building a specific request, get the params.
  const JsonValue* params = nullptr;
  if (auto ptr = objPtr->get("params")) {
    auto k = ptr->kind();
    if (k != JsonValue::Array && k != JsonValue::Object) {
      CHPLDEF_TODO();
      return nullptr;
    } else {
      params = ptr;
    }
  }

  switch (tag) {
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
    case name__: return name__::create(std::move(id), *params);
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }

  CHPLDEF_IMPOSSIBLE();

  return nullptr;
}

// TODO: Is outbound format the same as for incoming requests?
template <typename P, typename R>
JsonValue Request<P, R>::pack() const {
  return p.toJson();
}

template <typename T>
static Response createSuccessfulResponse(const T* t) {
  CHPL_ASSERT(t && t->isCompleted() && !t->hasError());
  auto r = t->result();
  CHPL_ASSERT(r);
  auto data = r->toJson();
  auto ret = Response::create(t->id(), std::move(data));
  return ret;
}

opt<Response> Message::response(Server* ctx, const Message* msg) {
  opt<Response> ret;

  if (msg->tag() == Message::UNSET || msg->tag() == Message::INVALID ||
      msg->tag() == Message::RESPONSE) {
    return ret;
  }

  switch (msg->status()) {
    case COMPLETED: {
      CHPL_ASSERT(!msg->hasError());
      switch (msg->tag()) {
        #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
        case name__: return createSuccessfulResponse(msg->to##name__());
        #include "./message-macro-list.h"
        #undef CHPLDEF_MESSAGE
        default: break;
      }
    } break;
    case FAILED: {
      CHPL_ASSERT(msg->hasError());
      JsonValue data(nullptr);
      ret = Response::create(msg->id(), msg->error(), msg->note(),
                             std::move(data));
    } break;
    default: break;
  }

  return ret;
}

bool Message::isOutbound() const {
  if (tag_ == Message::RESPONSE) return true;
  #define CHPLDEF_MESSAGE(name__, outbound__, x2__, x3__) \
  if (tag_ == Message::name__) return outbound__;
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  return false;
}

bool Message::isNotification() const {
  if (tag_ == Message::RESPONSE) return true;
  #define CHPLDEF_MESSAGE(name__, x1__, notification__, x3__) \
  if (tag_ == Message::name__) return notification__;
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  return false;
}

const char* Message::jsonRpcMethodName() const {
  return tagToJsonRpcMethodName(tag_);
}

const char* Message::tagToJsonRpcMethodName(Message::Tag tag) {
  switch (tag) {
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, rpc__) \
    case name__: return #rpc__;
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }

  return nullptr;
}

Message::Tag Message::jsonRpcMethodNameToTag(const char* str) {
  #define CHPLDEF_MESSAGE(name__, x1__, x2__, rpc__) \
  if (!strcmp(#rpc__, str)) return name__;
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  return Message::INVALID;
}

Message::Tag Message::jsonRpcMethodNameToTag(std::string str) {
  return jsonRpcMethodNameToTag(str.c_str());
}

std::string Message::idToString() const {
  if (auto optStr = id().getAsString()) return optStr->str();
  if (auto optInt = id().getAsInteger()) return std::to_string(*optInt);
  CHPLDEF_IMPOSSIBLE();
  return {};
}

template <typename P, typename R>
Message::Error Request<P, R>::unpack(const JsonValue& json, P& p,
                                     std::string* note) {
  llvm::json::Path::Root root;
  if (p.fromJson(json, root)) return Message::OK;
  *note = "Failed to unpack JSON";
  return Message::ERR_INVALID_PARAMS;
}

/** Define factory functions that unpack and construct the message. */
#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
chpl::owned<Message> name__::create(JsonValue id, const JsonValue& json) { \
  name__::Params p; \
  std::string note; \
  auto error = unpack(json, p, &note); \
  auto req = new name__(std::move(id), error, std::move(note), \
                        std::move(p)); \
  auto ret = chpl::toOwned(req); \
  return ret; \
}
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

static JsonObject
createResponseError(Message::Error e, std::string note,
                    JsonValue data) {
  JsonObject ret {
    { "code", Message::errorToInt(e) },
    { "message", std::move(note) }
  };

  if (data.kind() != JsonValue::Null) ret["data"] = std::move(data);
  return ret;
}

JsonValue Response::pack() const {
  JsonObject ret {
    { "jsonrpc", "2.0" },
    { "id", this->id() }
  };

  if (hasError()) {
    ret["error"] = createResponseError(error(), note(), data());
  } else {
    ret["result"] = data();
  }

  return ret;
}

Response Response::create(JsonValue id, JsonValue data) {
  CHPL_ASSERT(isIdValid(id));
  auto ret = Response(std::move(id), Message::OK, std::string(),
                      std::move(data));
  return ret;
}

Response Response::create(JsonValue id, Message::Error error,
                          std::string note,
                          JsonValue data) {
  CHPL_ASSERT(isIdValid(id) && error != Message::OK);
  auto ret = Response(std::move(id), error, std::move(note),
                      std::move(data));
  return ret;
}

template <typename P, typename R>
const R* Request<P, R>::result() const {
  if (status() != Message::COMPLETED) return nullptr;
  return &this->r;
}

const Response* Message::toResponse() const {
  if (tag_ == Message::RESPONSE) return (const Response*) this;
  return nullptr;
}

Response* Message::toResponse() {
  if (tag_ == Message::RESPONSE) return (Response*) this;
  return nullptr;
}

#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
const class name__* Message::to##name__() const { \
  if (tag_ == Message::name__) \
    return static_cast<const class name__*>(this); \
  return nullptr; \
} \
class name__* Message::to##name__() { \
  if (tag_ == Message::name__) return static_cast<class name__*>(this); \
  return nullptr; \
}
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

const char* Message::tagToString(Tag tag) {
  if (tag == Message::INVALID) return "Invalid";
  if (tag == Message::UNSET) return "Unset";
  if (tag == Message::RESPONSE) return "Response";
  #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
  if (tag == name__) return #name__;
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  return nullptr;
}

const char* Message::errorToString(Error error) {
  switch (error) {
    case OK: return "Ok";
    case ERR_PARSE: return "Parse";
    case ERR_INVALID_REQUEST: return "InvalidRequest";
    case ERR_METHOD_NOT_FOUND: return "MethodNotFound";
    case ERR_INVALID_PARAMS: return "InvalidParams";
    case ERR_INTERNAL: return "Internal";
    case ERR_SERVER_NOT_INITIALIZED: return "ServerNotInitialized";
    case ERR_UNKNOWN_ERROR_CODE: return "UnknownErrorCode";
    case ERR_REQUEST_FAILED: return "RequestFailed";
    case ERR_SERVER_CANCELLED: return "ServerCancelled";
    case ERR_CONTENT_MODIFIED: return "ContentModified";
    case ERR_REQUEST_CANCELLED: return "RequestCancelled";
  }

  CHPLDEF_IMPOSSIBLE();
  return nullptr;
}

namespace {
template <typename T, typename X>
static bool doHandleMessage(Server* ctx, T* msg, X& x) {
  if (msg->status() == Message::PROGRESSING) CHPLDEF_TODO();
  if (msg->status() != Message::PENDING) return true;

  ctx->message("Handling request '%s' with ID '%s'\n",
               msg->tagToString(),
               msg->idToString().c_str());

  x = msg->compute(ctx);

  if (x.isProgressingCallAgain) CHPLDEF_TODO();

  if (x.error != Message::OK) {
    auto cstr = Message::errorToString(x.error);
    ctx->message("Request failed with code '%s'\n", cstr);
  }

  if (ctx->logger().level() != Logger::OFF) {
    auto str = x.result.toString();
    ctx->message("Request completed with result %s\n", str.c_str());
  }

  return false;
}
}

/** Call the request handler above and then mark completed or failed. */
#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
void name__::handle(Server* ctx) { \
  ComputedResult x; \
  if (doHandleMessage(ctx, this, x)) { \
    this->r = std::move(x.result); \
    this->markCompleted(); \
  } else { \
    this->markFailed(x.error, std::move(x.note)); \
  } \
}
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

opt<Response> Message::handle(Server* ctx, Message* msg) {
  if (!ctx || !msg || msg->isResponse()) return {};
  if (msg->status() != Message::PENDING) return {};

  // TODO: Notification will have empty 'Result' type.
  if (msg->isNotification()) {
    CHPLDEF_TODO();
    return {};
  }

  msg->handle(ctx);

  if (msg->status() == Message::COMPLETED) {
    if (auto ret = Message::response(ctx, msg)) {
      CHPL_ASSERT(ret->id() == msg->id());
      CHPL_ASSERT(ret->status() == Message::COMPLETED);
      return ret;
    }
  }

  return {};
}

} // end namespace 'chpldef'
