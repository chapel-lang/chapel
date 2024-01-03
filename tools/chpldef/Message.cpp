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

#include "Message.h"
#include "Server.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <iostream>
#include <fstream>

namespace chpldef {

/** Forward declare specializations so that they are instantiated. This
    needs to be done so that these specializations are preferred by the
    linker. */
#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
  template chpl::owned<name__> \
  name__::create(JsonValue id, name__::Params p); \
  template chpl::owned<name__> \
  name__::createFromJson(JsonValue id, JsonValue j); \
  template opt<JsonValue> name__::pack() const; \
  template void name__::handle(Server* ctx); \
  template void name__::handle(Server* ctx, Response* r); \
  template void name__::handle(Server* ctx, name__::Result r); \
  template<> name__::ComputeResult \
  name__::compute(Server* ctx, name__::ComputeParams p); \
  template name__::~name__();
#include "message-macro-list.h"
#undef CHPLDEF_MESSAGE

bool Message::isIdValid(const JsonValue& id) {
  return id.kind() == JsonValue::Number ||
         id.kind() == JsonValue::String ||
         id.kind() == JsonValue::Null;
}

Message::Behavior Message::behavior(Tag tag) {
  switch (tag) {
    #define CHPLDEF_MESSAGE(name__, outbound__, notify__, rpc__) \
      case MessageTag::name__: \
        return determineBehavior(outbound__, notify__);
    #include "message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }
  return Message::NO_BEHAVIOR;
}

// TODO: Need to build an error message or record what went wrong. Create
// an 'Invalid' subclass and use that to record any error that occurs past
// the point of parsing the message ID.
chpl::owned<Message> Message::create(Server* ctx, JsonValue j) {
  auto objPtr = j.getAsObject();
  if (!objPtr) {
    ctx->verbose("Failed to unpack JSON object from %s\n",
                 jsonKindToString(j));
    return nullptr;
  }

  // Determine what method is to be called.
  auto tag = MessageTag::UNSET;
  if (auto optMethod = objPtr->getString("method")) {
    auto str = optMethod->str();

    // Compare against RPC names in 'message-macro-list.h'.
    tag = Message::jsonRpcMethodNameToTag(str);

    // TODO: We can create an 'Invalid' request to represent this failure.
    bool hasTag = tag != MessageTag::UNSET && tag != MessageTag::INVALID;
    if (!hasTag) {
      CHPLDEF_FATAL(ctx, "Unrecognized method '%s'\n", str.c_str());
      return nullptr;
    }
  }

  if (tag == MessageTag::RESPONSE) CHPLDEF_TODO();

  ctx->verbose("Constructing message with tag '%s'\n", tagToString(tag));

  // Get the ID as a JSON value.
  JsonValue id = nullptr;
  if (auto idPtr = objPtr->get("id")) {
    if (Message::isIdValid(*idPtr)) id = *idPtr;
  }

  auto k = id.kind();
  bool ok = true;
  ok &= !isNotification(tag) || k == JsonValue::Null;
  ok &= Message::isIdValid(id);

  // Error if we failed to get the ID.
  if (!ok) {
    ctx->verbose("Invalid message ID type '%s'\n",
                 jsonKindToString(k));
    return nullptr;
  }

  // Before building a specific request, get the params.
  JsonValue params(nullptr);
  if (auto fieldPtr = objPtr->get("params")) {
    auto& field = *fieldPtr;
    auto k = field.kind();
    if (k != JsonValue::Array && k != JsonValue::Object) {
      CHPLDEF_TODO();
      return nullptr;
    } else {
      std::swap(field, params);
    }
  }

  switch (tag) {
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
      case MessageTag::name__: { \
        return name__::createFromJson(std::move(id), std::move(params)); \
      } break;
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }

  CHPLDEF_IMPOSSIBLE();

  return nullptr;
}

template <typename M>
static opt<JsonValue> packIncomingRequest(const M* msg) {
  if (auto r = msg->result()) {
    if (msg->hasError()) {
      Response rsp(msg->id(), msg->error(), msg->note(), nullptr);
      return rsp.pack();
    } else {
      auto data = r->toJson();
      Response rsp(msg->id(), Message::OK, std::string(), std::move(data));
      return rsp.pack();
    }
  }
  return {};
}

template <typename M>
static opt<JsonValue> packIncomingNotify(const M* msg) {
  return {};
}

template <typename M>
static opt<JsonValue> packOutboundRequest(const M* msg) {
  JsonObject obj {
    { "jsonrpc", "2.0" },
    { "id", msg->id() },
  };

  if (auto p = msg->params()) {
    obj["params"] = p->toJson();
    JsonValue ret(std::move(obj));
    return ret;
  }

  return {};
}

template <typename M>
static opt<JsonValue> packOutboundNotify(const M* msg) {
  if (auto ret = packOutboundRequest(msg)) {
    if (auto obj = ret->getAsObject()) {
      obj->erase("id");
    }
    return ret;
  }
  return {};
}

template <Message::Tag T>
opt<JsonValue> TemplatedMessage<T>::pack() const {
  switch (behavior()) {
    case INCOMING_REQUEST: return packIncomingRequest(this);
    case INCOMING_NOTIFY: return packIncomingNotify(this);
    case OUTBOUND_REQUEST: return packOutboundRequest(this);
    case OUTBOUND_NOTIFY: return packOutboundNotify(this);
    default: break;
  }

  // All subclasses of 'TemplatedMessage' should have a behavior.
  CHPLDEF_IMPOSSIBLE();
  return {};
}

bool Message::isOutbound(Tag tag) {
  if (tag == MessageTag::RESPONSE) return true;
  #define CHPLDEF_MESSAGE(name__, outbound__, x2__, x3__) \
    if (tag == MessageTag::name__) return outbound__;
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  return false;
}

bool Message::isIncoming(Tag tag) {
  if (tag == MessageTag::RESPONSE) return true;
  return !isOutbound(tag);
}

bool Message::isNotification(Tag tag) {
  if (tag == MessageTag::RESPONSE) return false;
  #define CHPLDEF_MESSAGE(name__, x1__, notification__, x3__) \
    if (tag == MessageTag::name__) return notification__;
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
      case MessageTag::name__: return #rpc__;
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }

  return nullptr;
}

// TODO: Can use a trie if this becomes a performance bottleneck.
Message::Tag Message::jsonRpcMethodNameToTag(const char* str) {
  #define CHPLDEF_MESSAGE(name__, x1__, x2__, rpc__) \
    if (!strcmp(#rpc__, str)) return MessageTag::name__;
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  return MessageTag::INVALID;
}

Message::Tag Message::jsonRpcMethodNameToTag(std::string str) {
  return jsonRpcMethodNameToTag(str.c_str());
}

std::string Message::idToString() const {
  if (auto optStr = id().getAsString()) return optStr->str();
  if (auto optInt = id().getAsInteger()) return std::to_string(*optInt);
  if (id().kind() == JsonValue::Null) return "<null>";
  CHPLDEF_IMPOSSIBLE();
  return {};
}

template <MessageTag T>
Message::Error
TemplatedMessage<T>::unpack(JsonValue j, Params& p, std::string& note) {
  llvm::json::Path::Root root;
  if (p.fromJson(j, root)) return Message::OK;
  note = "Failed to unpack JSON for parameters";
  return Message::ERR_INVALID_PARAMS;
}

template <MessageTag K>
Message::Error
TemplatedMessage<K>::unpack(JsonValue j, Result& r, std::string& note) {
  llvm::json::Path::Root root;
  if (r.fromJson(j, root)) return Message::OK;
  note = "Failed to unpack JSON for result";
  return Message::ERR_INVALID_PARAMS;
}

template <MessageTag K>
Message::Error
TemplatedMessage<K>::unpack(Response* rsp, Result& r, std::string& note) {
  CHPLDEF_TODO();
  return Message::OK;
}

template <MessageTag K>
chpl::owned<TemplatedMessage<K>>
TemplatedMessage<K>::create(JsonValue id, Params p) {
  auto msg = new TemplatedMessage<K>(K, std::move(id), Message::OK, {},
                                     std::move(p));
  auto ret = chpl::toOwned<>(msg);
  return ret;
}

template <MessageTag K>
chpl::owned<TemplatedMessage<K>>
TemplatedMessage<K>::createFromJson(JsonValue id, JsonValue j) {
  Params p = {};
  std::string note;
  auto error = unpack(std::move(j), p, note);
  auto msg = new TemplatedMessage<K>(K, std::move(id), error,
                                     std::move(note),
                                     std::move(p));
  auto ret = chpl::toOwned<>(msg);
  return ret;
}

static JsonObject
packResponseError(Message::Error e, std::string note, JsonValue data) {
  JsonObject ret {
    { "code", Message::errorToInt(e) },
    { "message", std::move(note) }
  };

  if (data.kind() != JsonValue::Null) ret["data"] = std::move(data);
  return ret;
}

opt<JsonValue> Response::pack() const {
  JsonObject obj {
    { "jsonrpc", "2.0" },
    { "id", this->id() }
  };

  if (hasError()) {
    obj["error"] = packResponseError(error(), note(), data());
  } else {
    obj["result"] = data();
  }

  JsonValue ret(std::move(obj));
  return ret;
}

chpl::owned<Message> Response::create(JsonValue id, JsonValue data) {
  CHPL_ASSERT(isIdValid(id));
  auto rsp = new Response(std::move(id), Message::OK, std::string(),
                          std::move(data));
  auto ret = chpl::toOwned(rsp);
  return ret;
}

chpl::owned<Message> Response::create(JsonValue id, Message::Error error,
                                      std::string note,
                                      JsonValue data) {
  CHPL_ASSERT(isIdValid(id) && error != Message::OK);
  auto rsp = new Response(std::move(id), error, std::move(note),
                          std::move(data));
  auto ret = chpl::toOwned(rsp);
  return ret;
}

const Response* Message::toResponse() const {
  if (tag_ == MessageTag::RESPONSE)
    return static_cast<const Response*>(this);
  return nullptr;
}

Response* Message::toResponse() {
  if (tag_ == MessageTag::RESPONSE)
    return static_cast<Response*>(this);
  return nullptr;
}

#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
  const name__* Message::to##name__() const { \
    if (tag_ == MessageTag::name__) \
      return static_cast<const name__*>(this); \
    return nullptr; \
  } \
  name__* Message::to##name__() { \
    if (tag_ == MessageTag::name__) \
      return static_cast<name__*>(this); \
    return nullptr; \
  }
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

const char* Message::tagToString(Tag tag) {
  if (tag == MessageTag::UNSET) return "Unset";
  if (tag == MessageTag::INVALID) return "Invalid";
  if (tag == MessageTag::RESPONSE) return "Response";
  #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
    if (tag == MessageTag::name__) return #name__;
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

const char* Message::behaviorToString(Message::Behavior b) {
  switch (b) {
    case Message::INCOMING_REQUEST: return "incoming request";
    case Message::INCOMING_NOTIFY: return "incoming notification";
    case Message::OUTBOUND_REQUEST: return "outbound request";
    case Message::OUTBOUND_NOTIFY: return "outbound notification";
    case Message::NO_BEHAVIOR: return "none";
  }

  return nullptr;
}

// Use this pattern so that we may implement the TemplatedMessage class with
// an unbounded number of implementation methods that are not documented
// in the header. Because the handler is marked 'friend', it can access the
// message state.
template <typename M>
class TemplatedMessageHandler {
public:
  static constexpr auto BEHAVIOR = M::BEHAVIOR;
  using Params = typename M::Params;
  using Result = typename M::Result;
  using ComputeResult = typename M::ComputeResult;
private:
  Server* ctx_ = nullptr;
  M* msg_ = nullptr;
public:
  TemplatedMessageHandler(Server* ctx, M* msg) : ctx_(ctx), msg_(msg) {}
  TemplatedMessageHandler() = default;

  inline std::string fmt() { return ctx_->fmt(static_cast<Message*>(msg_)); }
  inline const char* dsc() const {
    constexpr bool hasBehavior = BEHAVIOR != Message::NO_BEHAVIOR;
    auto ret = hasBehavior ? Message::behaviorToString(BEHAVIOR) : "message";
    return ret;
  }

  void logComputationPrelude() {
    ctx_->message("Handling %s '%s'\n", dsc(), fmt().c_str());
  }

  void logComputationEpilogue(const ComputeResult& cr) {
    auto dsc = this->dsc();
    auto fmt = this->fmt();
    if (cr.error != Message::OK) {
      ctx_->message("The %s %s failed with code '%s'\n", dsc, fmt.c_str(),
                    Message::errorToString(cr.error));
    } else {
      ctx_->message("The %s '%s' is complete...\n", dsc, fmt.c_str());
    }
  }

  // This pattern of constexpr folding is used over specialization because
  // of defect CWG727 (cannot specialize in class scope) which still has
  // not been fixed by GCC.
  template <Message::Behavior B>
  void issueComputationCall() {
    if constexpr (B == Message::INCOMING_REQUEST) {
      computeIncomingRequest();
    } else if constexpr (B == Message::INCOMING_NOTIFY) {
      computeIncomingNotify();
    // Outbound messages cannot be handled without a result.
    } else {
      std::abort();
    }
  }

  template <Message::Behavior B>
  void issueComputationCall(Result r) {
    if constexpr (B == Message::OUTBOUND_REQUEST) {
      computeOutboundRequest(std::move(r));
    // Outbound notifications should never be handled.
    } else {
      std::abort();
    }
  }

  void computeIncomingRequest() {
    logComputationPrelude();
    auto cr = M::compute(ctx_, msg_->p);
    logComputationEpilogue(cr);
    if (cr.isProgressingCallAgain) CHPLDEF_TODO();
    if (cr.error != Message::OK) {
      msg_->markFailed(cr.error, std::move(cr.note));
    } else {
      msg_->r = std::move(cr.result);
      msg_->markCompleted();
    }
  }

  void computeIncomingNotify() {
    computeIncomingRequest();
  }

  void computeOutboundRequest(Result r) {
    logComputationPrelude();
    std::swap(msg_->r, r);
    auto cr = M::compute(ctx_, msg_->r);
    logComputationEpilogue(cr);
    if (cr.isProgressingCallAgain) CHPLDEF_TODO();
    if (cr.error != Message::OK) {
      msg_->markFailed(cr.error, std::move(cr.note));
    } else {
      msg_->markCompleted();
    }
  }

  void handle() {
    if (msg_->isOutbound() || msg_->isDone()) return;
    if (msg_->status() == Message::PROGRESSING) CHPLDEF_TODO();
    issueComputationCall<BEHAVIOR>();
  }

  void handle(Response* rsp) {
    if (!rsp || rsp->id() != msg_->id()) return;
    if (rsp->status() == Message::FAILED) CHPLDEF_TODO();
    std::string note;
    Result r;
    auto error = M::unpack(rsp, r, note);
    if (error != Message::OK) {
      msg_->markFailed(error, std::move(note));
    } else {
      handle(std::move(r));
    }
  }

  void handle(Result r) {
    if (msg_->isOutbound() || msg_->isDone()) return;
    if (msg_->status() == Message::PROGRESSING) CHPLDEF_TODO();
    issueComputationCall<BEHAVIOR>(std::move(r));
  }
};

template <MessageTag K>
void TemplatedMessage<K>::handle(Server* ctx) {
  TemplatedMessageHandler<TemplatedMessage<K>> tmh(ctx, this);
  tmh.handle();
}

template <MessageTag K>
void TemplatedMessage<K>::handle(Server* ctx, Response* rsp) {
  TemplatedMessageHandler<TemplatedMessage<K>> tmh(ctx, this);
  tmh.handle(rsp);
}

template <MessageTag K>
void TemplatedMessage<K>::handle(Server* ctx, Result r) {
  TemplatedMessageHandler<TemplatedMessage<K>> tmh(ctx, this);
  tmh.handle(std::move(r));
}

} // end namespace 'chpldef'
