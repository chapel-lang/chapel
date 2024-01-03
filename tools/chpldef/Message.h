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

#ifndef CHPL_TOOLS_CHPLDEF_MESSAGE_H
#define CHPL_TOOLS_CHPLDEF_MESSAGE_H

#include "misc.h"
#include "protocol-types.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"
#include <cstdint>
#include <iostream>
#include <fstream>

namespace chpldef {

class Response;
class Server;
class Message;

/** These tags are used to do dynamic casts to message types at runtime,
    and to parameterize `TemplatedMessage` instances at compile-time. */
enum class MessageTag {
  UNSET                 = 0,
  INVALID               = 1,
  RESPONSE              = 2,
  // Expand the message macros.
  #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) name__ ,
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE
  NUM_MESSAGES
};

template <MessageTag> class TemplatedMessage;

/** Attempts to model a LSP message. A message may be either incoming or
    outgoing (most are incoming). */
class Message {
public:
  using Tag = MessageTag;

  /** Error codes are listed in order according to the LSP spec. */
  enum Error {
    OK                            = 0,
    ERR_PARSE                     = -32700,
    ERR_INVALID_REQUEST           = -32600,
    ERR_METHOD_NOT_FOUND          = -32601,
    ERR_INVALID_PARAMS            = -32602,
    ERR_INTERNAL                  = -32603,
    ERR_SERVER_NOT_INITIALIZED    = -32002,
    ERR_UNKNOWN_ERROR_CODE        = -32001,
    ERR_REQUEST_FAILED            = -32803,
    ERR_SERVER_CANCELLED          = -32802,
    ERR_CONTENT_MODIFIED          = -32801,
    ERR_REQUEST_CANCELLED         = -32800
  };

  enum Status {
    PENDING,        /** This message has not been handled yet. */
    PROGRESSING,    /** This message is being handled. */
    COMPLETED,      /** This message has been handled. */
    FAILED          /** Handling of this message failed. */
  };

  /** Behavior is a combination of message direction and return value. */
  enum Behavior {
    NO_BEHAVIOR,
    INCOMING_REQUEST,
    INCOMING_NOTIFY,
    OUTBOUND_REQUEST,
    OUTBOUND_NOTIFY
  };

private:
  Tag tag_ = MessageTag::UNSET;
  JsonValue id_ = nullptr;
  Error error_ = Message::OK;
  Status status_ = Message::PENDING;
  std::string note_;

protected:
  Message(Message::Tag tag, JsonValue id, Error error,
          std::string note)
      : tag_(tag),
        id_(std::move(id)),
        error_(error),
        note_(std::move(note)) {
    CHPL_ASSERT(isIdValid(id_));
    if (id.kind() == JsonValue::Null) CHPL_ASSERT(isNotification());
    if (error != Message::OK) status_ = Message::FAILED;
  }

  inline void markProgressing() { status_ = Message::PROGRESSING; }
  inline void markCompleted() { status_ = Message::COMPLETED; }
  inline void markFailed(Error error, std::string note=std::string()) {
    status_ = Message::FAILED;
    error_ = error;
    note_ = std::move(note);
  }

  static bool isIdValid(const JsonValue& id);

public:
  virtual ~Message() = default;

  /** Create a request given a JSON value. */
  static chpl::owned<Message> create(Server* ctx, JsonValue j);

  /** The tag for this message. */
  inline Tag tag() const { return tag_; }

  /** Print a message tag as a string. */
  static const char* tagToString(Tag tag);

  /** Print this message's tag as a string. */
  inline const char* tagToString() const { return tagToString(tag_); }

  /** Returns 'true' if this message has an error value. */
  inline bool hasError() const { return error_ != Message::OK; }

  /** If this message failed, return the reason why. */
  inline Message::Error error() const { return error_; }

  /** Print a message error code as a string. */
  static const char* errorToString(Error error);

  /** Print this message's error code as a string. */
  inline const char* errorToString() const { return errorToString(error_); }

  /** Get the numeric value of an error code. */
  static inline int64_t errorToInt(Error e) {
    return static_cast<int64_t>(e);
  }

  /** Get the numeric value of this message's error code. */
  inline int64_t errorToInt() const { return errorToInt(error_); }

  /** If this message failed, return a possible note explaining why. */
  const std::string& note() const { return note_; }

  /** Return the current status of this message. */
  inline Message::Status status() const { return status_; }

  /** Returns 'true' if this message is marked 'COMPLETED'. */
  inline bool isCompleted() const { return status_ == Message::COMPLETED; }

  /** Returns 'true' if this message is marked 'FAILED'. */
  inline bool isFailed() const { return status_ == Message::FAILED; }

  /** Returns 'true' if this message is marked 'COMPLETED' or 'FAILED'. */
  inline bool isDone() const { return isCompleted() || isFailed(); }

  /** If 'true', this message is a response. */
  inline bool isResponse() const { return tag_ == MessageTag::RESPONSE; }

  /** If 'true', then 'tag' is for a message being sent to the server. */
  static bool isOutbound(Tag tag);

  /** If 'true', the server is sending this message to the client. */
  inline bool isOutbound() const { return isOutbound(tag_); }

  /** If 'true', then 'tag' is for a message being sent to the server. */
  static bool isIncoming(Tag tag);

  /** If 'true', the client is sending this message to the server. */
  inline bool isIncoming() const { return isIncoming(tag_); }

  /** If 'true', then the Tag 'tag' does not need a response. */
  static bool isNotification(Tag tag);

  /** If 'true', then this message does not need a response. */
  inline bool isNotification() const { return isNotification(tag_); }

  /** Return the behavior of a message tag. */
  static Behavior behavior(Tag tag);

  /** Return the behavior of this message. */
  inline Behavior behavior() const { return behavior(tag_); }

  /** Print a behavior value as a string. */
  static const char* behaviorToString(Behavior b);

  /** Returns the expected JSON-RPC method name for this message. */
  const char* jsonRpcMethodName() const;

  /** Returns the message ID. */
  inline const JsonValue& id() const { return id_; }

  /** Returns the message ID as a string. */
  std::string idToString() const;

  /** Convert a given message tag to string. */
  static const char* tagToJsonRpcMethodName(Tag tag);

  /** Convert from JSON-RPC method name to message tag. */
  static Tag jsonRpcMethodNameToTag(const char* str);

  /** Convert from JSON-RPC method name to message tag. */
  static Tag jsonRpcMethodNameToTag(std::string str);

  /** Dynamic cast to a response. */
  const Response* toResponse() const;

  /** Dynamic cast to a response, mutable. */
  Response* toResponse();

  /** Dynamic cast to other message subclasses, const and non-const. */
  #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
    const TemplatedMessage<MessageTag::name__>* to##name__() const; \
    TemplatedMessage<MessageTag::name__>* to##name__();
  #include "./message-macro-list.h"
  #undef CHPLDEF_MESSAGE

  /** Base visitor over messages, does nothing and returns default value. */
  template <typename T>
  class Visitor {
  private:
    Server* ctx_;

  public:
    Visitor(Server* ctx) : ctx_(ctx) {}

    virtual T visit(const class Response* rsp) {
      T ret;
      return ret;
    }

    /** Default visitor body is trivial enough that we define it here. */
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
      virtual T visit(const TemplatedMessage<MessageTag::name__>* req) { \
        T ret; return ret; \
      }
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE

    /** Invoke the visitor on a given message, defined below. */
    T dispatch(const Message* req);

    /** Get the server context. */
    Server* ctx() const;
  };

  /** Determine a message's behavior. */
  static inline constexpr Message::Behavior
  determineBehavior(bool outbound, bool notify) {
    return outbound
      ? (notify ? Message::OUTBOUND_NOTIFY : Message::OUTBOUND_REQUEST)
      : (notify ? Message::INCOMING_NOTIFY : Message::INCOMING_REQUEST);
  }

  /** Dispatch a visitor using a message as the receiver. */
  template <typename T>
  inline T accept(Message::Visitor<T>& v) { return v.dispatch(*this); }

  /** Some messages can defer, but not sure which at this moment. */
  virtual bool defer() const { return false; }

  /** Pack this message into a JSON value. */
  virtual opt<JsonValue> pack() const = 0;

  /** For incoming requests, compute a result using the input parameters. */
  virtual void handle(Server* ctx) = 0;

  /** For outbound requests, compute using a response sent by the client. */
  virtual void handle(Server* ctx, Response* rsp) = 0;
};

namespace detail {

/** Stores computation results for a message. */
template <typename R>
struct ComputationOutput {
  bool isProgressingCallAgain = false;
  Message::Error error = Message::OK;
  std::string note;
  R result;

  /** For convenience when implementing 'compute' for a request. */
  ComputationOutput(R&& result)
      : isProgressingCallAgain(false),
        error(Message::OK),
        result(result) {}

  ComputationOutput(bool isProgressingCallAgain, Message::Error error,
                    std::string note,
                    R result)
      : isProgressingCallAgain(isProgressingCallAgain),
        error(error),
        note(std::move(note)),
        result(std::move(result)) {}

  ComputationOutput() = default;
};

/** Message specializations return this in absence of another value. */
using Empty = EmptyProtocolType;

template<typename P, typename R, Message::Behavior B>
struct Computation {};

/** Computation details for incoming requests. */
template <typename P, typename R>
struct Computation<P, R, Message::INCOMING_REQUEST> {
  static constexpr auto BEHAVIOR = Message::INCOMING_REQUEST;
  using Params = P;
  using Result = R;
  using FunctionParams = const Params&;
  using FunctionResult = ComputationOutput<Result>;
};

/** Computation details for incoming notifications. */
template <typename P, typename R>
struct Computation<P, R, Message::INCOMING_NOTIFY> {
  static constexpr auto BEHAVIOR = Message::INCOMING_NOTIFY;
  using Params = P;
  using Result = Empty;
  using FunctionParams = const Params&;
  using FunctionResult = ComputationOutput<Result>;
};

/** Computation details for outbound requests. */
template <typename P, typename R>
struct Computation<P, R, Message::OUTBOUND_REQUEST> {
  static constexpr auto BEHAVIOR = Message::OUTBOUND_REQUEST;
  using Params = P;
  using Result = R;
  using FunctionParams = const Result&;
  using FunctionResult = ComputationOutput<Empty>;
};

/** Details for outbound notifications. Most fields are an empty struct
    because outbound notifications should never be computed (they are
    only ever created then sent off to the client. Any validation must be
    done by the entity that created one). */
template <typename P, typename R>
struct Computation<P, R, Message::OUTBOUND_NOTIFY> {
  static constexpr auto BEHAVIOR = Message::OUTBOUND_NOTIFY;
  using Params = P;
  using Result = Empty;
  using FunctionParams = Empty;
  using FunctionResult = ComputationOutput<Empty>;
};

template <MessageTag K> struct ComputationByTag {};

/** Specialize computation details for each tag. */
#define CHPLDEF_MESSAGE(name__, outbound__, notify__, x3__) \
  template <> struct ComputationByTag<MessageTag::name__> { \
    static constexpr \
    auto B = Message::determineBehavior(outbound__, notify__); \
    using type = Computation<name__##Params, name__##Result, B>; \
  };
#include "message-macro-list.h"
#undef CHPLDEF_MESSAGE

} // end namespace 'detail'

template <typename M>
class TemplatedMessageHandler;

template <MessageTag K>
class TemplatedMessage : public Message {
public:
  using Computation = typename detail::ComputationByTag<K>::type;
  static constexpr auto BEHAVIOR = Computation::BEHAVIOR;
  using Params = typename Computation::Params;
  using Result = typename Computation::Result;
  using ComputeParams = typename Computation::FunctionParams;
  using ComputeResult = typename Computation::FunctionResult;
  using Self = TemplatedMessage<K>;
protected:
  friend TemplatedMessageHandler<TemplatedMessage<K>>;

  const Params p;
  Result r = {};

  TemplatedMessage(Message::Tag tag, JsonValue id, Message::Error error,
                   std::string note,
                   Params p)
      : Message(tag, std::move(id), error, std::move(note)),
        p(std::move(p)) {}

  static Message::Error unpack(JsonValue j, Params& p, std::string& note);
  static Message::Error unpack(JsonValue j, Result& r, std::string& note);
  static Message::Error unpack(Response* rsp, Result& r, std::string& note);

  /** Use in message handlers to return failure. */
  static ComputeResult fail(Error error=Message::ERR_REQUEST_FAILED,
                     std::string note=std::string()) {
    return { false, error, std::move(note), {} };
  }

  /** Use in message handlers to delay. */
  static ComputeResult delay() {
    return { true, Message::OK, {}, {} };
  }

public:
  // Cannot use 'default' because of a conflict with specialization on GCC.
  virtual ~TemplatedMessage() override {}

  /** Create a message using a JSON id and parameters. */
  static chpl::owned<Self> create(JsonValue id, Params p);

  /** Create a message using a JSON id, error code and optional note. */
  static chpl::owned<Self> createFromJson(JsonValue id, JsonValue j);

  /** Pack this request into a JSON value. The contents of the JSON value
      depend on the behavior of this message. There are four modes:
      a message can either be a notification or a request, and it can either
      be incoming (client-to-server) or outbound (server-to-client).

      A message should only be packed if it is marked 'COMPLETE'. Otherwise,
      the contents of the JSON may not be valid.

      For incoming requests, the request result is implicitly wrapped in a
      message of type 'Response', and then that Response is packed into
      JSON. This is to conform with the LSP protocol (it is as if we are
      sending the return value of a function call to the client).

      For outgoing requests and notifications, the message parameters are
      packed directly into JSON.

      For incoming notifications, a JSON 'null' value is returned, as there
      is nothing to pack.
  */
  virtual opt<JsonValue> pack() const final override;

  /** For incoming messages, compute a result using the input parameters. */
  virtual void handle(Server* ctx) final override;

  /** For outbound messages, compute using a response sent by the client. */
  virtual void handle(Server* ctx, Response* r) final override;

  /** For outbound messages, compute using results sent by the client. */
  void handle(Server* ctx, Result r);

  /** Get the parameters of this message if they are valid. */
  inline const Params* params() const {
    if (status() == Message::FAILED) return nullptr;
    return &this->p;
  }

  /** Get the result of this message if it was computed. */
  inline const Result* result() const {
    if (status() != Message::COMPLETED) return nullptr;
    return &this->r;
  }

  /** This function performs the actual computation. */
  static ComputeResult compute(Server* ctx, ComputeParams p);
};

/** Create aliases for each specialization over a message tag. */
#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
  using name__ = class TemplatedMessage<MessageTag::name__>;
#include "message-macro-list.h"
#undef CHPLDEF_MESSAGE

/** Deliberately specialize these tags to be empty. */
template <>
class TemplatedMessage<MessageTag::RESPONSE> {};
template <>
class TemplatedMessage<MessageTag::INVALID> {};
template <>
class TemplatedMessage<MessageTag::NUM_MESSAGES> {};

/** The structure of a response is simple and rigid. When a incoming request
    is packed into JSON, it is implicitly wrapped in a response. When the
    server sends the client an outbound request, it registers the request
    and awaits a response.
*/
class Response : public Message {
private:
  JsonValue data_;

public:
  Response(JsonValue id, Message::Error error, std::string note,
           JsonValue data)
      : Message(MessageTag::RESPONSE, std::move(id), error, std::move(note)),
        data_(data) {
    CHPL_ASSERT(this->id().kind() != JsonValue::Null);
    CHPL_ASSERT(isResponse() && isOutbound());
    this->markCompleted();
  }

  virtual ~Response() = default;

  /** Create a response given an ID and a result value. */
  static chpl::owned<Message> create(JsonValue id, JsonValue data=nullptr);

  /** Create a response given an ID, an error code, and optional details.
      If 'data' is 'nullptr' then it will not be included in the
      JSON created when this message is packed. Notes will be included
      even if they are the empty string, as this is required by the
      protocol. */
  static chpl::owned<Message> create(JsonValue id, Message::Error error,
                                     std::string note=std::string(),
                                     JsonValue data=nullptr);

  /** Create a response by unpacking it from JSON. */
  static chpl::owned<Message> create(JsonValue j);

  /** Get the JSON data this response stores. */
  inline const JsonValue& data() const { return data_; }

  /** Pack this response into a JSON value. Messages have a rigid format,
      so the layout of the JSON sould always be the same sans omission
      of optional fields (see the LSP spec section on 'Response' messages). */
  virtual opt<JsonValue> pack() const override;

  /** Handling a response does nothing. */
  virtual void handle(Server* ctx) override {}

  /** Handling a response does nothing. */
  virtual void handle(Server* ctx, Response* rsp) override {}
};

/** Define visitor dispatch now that all subclasses have been defined. */
template <typename T>
T Message::Visitor<T>::dispatch(const Message* req) {
  switch (req->tag()) {
    case MessageTag::RESPONSE: {
      return visit(req->toResponse());
    } break;
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
      case MessageTag::name__: { \
        using F##name__ = TemplatedMessage<MessageTag::name__>; \
        auto c = static_cast<const F##name__*>(req); \
        return visit(c); \
      } break;
    #include "message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }
  CHPLDEF_IMPOSSIBLE();
  T ret;
  return ret;
}

} // end namespace 'chpldef'

#endif
