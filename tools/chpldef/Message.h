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

#ifndef CHPL_TOOLS_CHPLDEF_MESSAGE_H
#define CHPL_TOOLS_CHPLDEF_MESSAGE_H

#include "./misc.h"
#include "./protocol-types.h"
#include "./Server.h"

#include "llvm/Support/FileSystem.h"
#include "llvm/Support/JSON.h"

#include <cstdint>
#include <iostream>
#include <fstream>

namespace chpldef {

template <typename P, typename R>
class Request;
class Response;

/** Forward declare requests. */
#define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) class name__;
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

/**
  Attempts to model a LSP message. A message may be either incoming or
  outgoing (most are incoming).
*/
class Message {
public:

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

  enum Tag {
    UNSET                 = 0,
    INVALID               = 1,
    RESPONSE              = 2,

    // Expand the message macros.
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) name__ ,
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    NUM_MESSAGES
  };

private:
  Tag tag_;
  JsonValue id_;
  Error error_ = Message::OK; 
  Status status_ = Message::PENDING;
  std::string note_;

protected:
  inline void markProgressing() { status_ = Message::PROGRESSING; }
  inline void markCompleted() { status_ = Message::COMPLETED; }
  inline void markFailed(Error error, std::string note=std::string()) {
    status_ = Message::FAILED;  
    error_ = error;
    note_ = std::move(note);
  }

  static bool isIdValid(const JsonValue& id);

  Message(Message::Tag tag, JsonValue&& id, Error error,
          std::string note)
      : tag_(tag),
        id_(std::move(id)),
        error_(error),
        note_(std::move(note)) {
    CHPL_ASSERT(isIdValid(id_));
  }

public:
  virtual ~Message() = default;

  /** Create a request given a JSON value. */
  static chpl::owned<Message> request(Server* ctx, JsonValue&& json);

  /** Create a response to a handled message. The status of the message 
      must be COMPLETED or FAILED, or else nothing is returned. */
  static opt<Response> response(Server* ctx, const Message* msg);

  /** The tag for this message. */
  inline Tag tag() const { return tag_; }

  /** Returns 'true' if this message has an error value. */
  inline bool hasError() const { return error_ != Message::OK; }

  /** If this message failed, return the reason why. */
  inline Message::Error error() const { return error_; }

  /** If this message failed, return a possible note explaining why. */
  const std::string& note() const { return note_; }

  /** Get the numeric value of an error code. */
  static inline int64_t errorToInt(Error e) {
    return static_cast<int64_t>(e);
  }

  /** Get the numeric value of an error code. */
  inline int64_t errorToInt() const { return errorToInt(error_); }

  /** Return the current status of this message. */
  inline Message::Status status() const { return status_; }

  /** Returns 'true' if this message is marked 'COMPLETED'. */
  inline bool isCompleted() const { return status_ == Message::COMPLETED; }

  /** If 'true', this message is a response. */
  inline bool isResponse() const { return tag_ == Message::RESPONSE; }

  /** If 'true', the server is sending this message to the client. */
  bool isOutbound() const;

  /** If 'true', then this message does not need a response. */
  bool isNotification() const;

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
  const class name__* to##name__() const; \
  class name__* to##name__();
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
    virtual T visit(const class name__* req) { \
      T ret; \
      return ret; \
    }
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE

    /** Invoke the visitor on a given message, defined below. */
    T dispatch(const Message* req);

    /** Get the server context. */
    Server* ctx() const;
  };

  /** Dispatch a visitor using a message as the receiver. */
  template <typename T, typename U>
  inline T accept(Message::Visitor<U>& v) { return v.dispatch(*this); }

  /** Some messages can defer, but not sure which at this moment. */
  virtual bool defer() const { return false; }

  /** Pack this message into a JSON value. */
  virtual JsonValue pack() const = 0;

  /** May handle a message, but does not send or receive. */
  static opt<Response> handle(Server* ctx, Message* msg);
};

/** Most messages are incoming and are modelled as a 'Request'. A request
    consists of a set of parameter values, that can be used to compute
    a result (it is analogous to a function call). Some requests can be
    sent by the server to the client, in which case they are 'sendable',
    and should provide a definition for the 'pack()' method.
*/
template <typename Params, typename Result>
class Request : public Message {
public:
  /** Stores computation results for this request. */
  struct ComputedResult {
    bool isProgressingCallAgain;
    Message::Error error;
    std::string note;
    Result result;
  };

protected:
  const Params p;
  Result r = {};

  static Message::Error unpack(const JsonValue& json, Params& p,
                               std::string* note);

  Request(Message::Tag tag, JsonValue&& id, Message::Error error,
          std::string&& note,
          Params&& params)
      : Message(tag, std::move(id), error, std::move(note)),
        p(params) {}

  /** Use in message handlers to return failure. */
  inline ComputedResult fail(Error error=Message::ERR_REQUEST_FAILED,
                             std::string&& note=std::string()) const {
    return { false, error, std::move(note), {} };
  }

  /** Use in message handlers to delay. */
  inline ComputedResult delay() const {
    return { true, Message::OK, {}, {} };
  }

  /** Use in message handlers to return success. */
  inline ComputedResult succeed(Result&& ret) const {
    return { false, Message::OK, {}, std::move(ret) };
  }

public:
  virtual ~Request() = default;

  /** Pack the _parameters_ of this request into a JSON value. To pack
      the result of a request into a JSON value, either do so manually
      or create a Response. This is method is used for sending outbound
      requests from server to client. */
  virtual JsonValue pack() const;

  /** Compute the answer to this request, doing meaningful work. */
  virtual ComputedResult compute(Server* ctx) = 0;

  /** Compute results and store them in this request for later use. */
  void handle(Server* ctx);

  /** If computed, get the result of this request. */
  const Result* result() const;
};

/** Expand each LSP message into its own subclass of Request. Each message
    'Foo' has a parameter type and return type, 'FooParams' and 'FooResult',
    which can be found in the 'protocol-types.h' header. These names are
    internally consistent but may not always match the spec - a small set of
    protocol types defined in the LSP spec do not always match this pattern.

    When in doubt and referring to the spec, use the "JSON-RPC method name"
    to look up the message, as that is unambiguous.
*/
#define CHPLDEF_MESSAGE(name__, outbound__, notification__, rpc__) \
class name__ : public Request<name__##Params, name__##Result> { \
public: \
  using Params = name__##Params; \
  using Result = name__##Result; \
  using ComputedResult = Request<Params, Result>::ComputedResult; \
private: \
  name__(JsonValue&& id, Message::Error error, std::string&& note, \
         Params&& p) \
      : Request(Message::name__, std::move(id), error, \
                std::move(note), \
                std::move(p)) {} \
public: \
  virtual ~name__() = default; \
  static chpl::owned<Message> create(JsonValue&& id, const JsonValue& json); \
  virtual ComputedResult compute(Server* ctx); \
};
#include "./message-macro-list.h"
#undef CHPLDEF_MESSAGE

/** One response is created for every request message. The structure of a
    response is simple and rigid when compared to inbound (client-to-server)
    requests. Because there is a 1-to-1 relationship between messages and
    responses, a message/response pair must share IDs. Responses are always
    outbound and cannot be handled by the server (there is no work to do
    other than send the response).
*/
class Response : public Message {
private:
  JsonValue data_;

  Response(JsonValue&& id, Message::Error error, std::string&& note,
           JsonValue&& data)
      : Message(Message::RESPONSE, std::move(id), error, std::move(note)),
        data_(data) {
    CHPL_ASSERT(isResponse() && isOutbound());
  }

public:
  virtual ~Response() = default;

  /** Get the JSON data this response stores. */
  inline const JsonValue& data() const { return data_; }

  /** Pack this response into a JSON value. */
  virtual JsonValue pack() const;

  // Note no '&&' references for these formals, since the most common case
  // when creating a response is to create it based off an existing
  // request (so we have to make some copies).

  /** Create a response given an ID and a result value. */
  static Response create(JsonValue id, JsonValue data=nullptr);

  /** Create a response given an ID, an error code, and optional details.
      If 'data' is 'nullptr' then it will not be included in the
      JSON created when this message is packed. Notes will be included
      even if they are the empty string, as this is required by the
      protocol. */
  static Response create(JsonValue id, Message::Error error,
                         std::string note=std::string(),
                         JsonValue data=nullptr);
};

/** Define visitor dispatch now that all subclasses have been defined. */
template <typename T>
T Message::Visitor<T>::dispatch(const Message* req) {
  switch (req->tag()) {
    case RESPONSE: {
      return visit(req->toResponse());
    } break;
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
    case name__: { \
      auto casted = static_cast<const class name__*>(req); \
      return visit(casted); \
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
