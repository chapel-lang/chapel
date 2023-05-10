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

/** This is one file where message handlers can be implemented. However, if
    a particular message's handler grows to be very large in size (e.g.,
    perhaps 500+ lines), then we should feel free to move it to a different
    source file with a name that matches the message name. */
namespace chpldef {

template <typename P, typename R>
void Request<P, R>::handle(Server* ctx) {
  if (status() == Message::PROGRESSING) CHPLDEF_TODO();
  if (status() != Message::PENDING) return;

  auto x = this->compute(ctx);

  if (x.isProgressingCallAgain) CHPLDEF_TODO();

  if (x.error != Message::OK) {
    this->markFailed(x.error, std::move(x.note));
  } else {
    this->markCompleted();
    this->r = std::move(x.result);
  }
}

opt<Response> Message::handle(Server* ctx, Message* msg) {
  if (!ctx || !msg || msg->isResponse()) return {};
  if (msg->status() != Message::PENDING) return {};

  // TODO: Notification will have empty 'Result' type.
  if (msg->isNotification()) {
    CHPLDEF_TODO();
    return {};
  }

  switch (msg->tag()) {
    #define CHPLDEF_MESSAGE(name__, x1__, x2__, x3__) \
    case name__: { \
      auto p = msg->to##name__(); \
      CHPL_ASSERT(p); \
      p->handle(ctx); \
    } break;
    #include "./message-macro-list.h"
    #undef CHPLDEF_MESSAGE
    default: break;
  }

  if (msg->status() == Message::COMPLETED) {
    if (auto ret = Message::response(ctx, msg)) {
      CHPL_ASSERT(ret->id() == msg->id());
      CHPL_ASSERT(ret->status() == Message::COMPLETED);
      return ret;
    }
  }

  return {};
}

/** TODO: Fill in 'InitializeResult', turning most fields off. */
Initialize::ComputedResult Initialize::compute(Server* ctx) {
  Result ret;
  return ret;
}

} // end namespace 'chpldef'
