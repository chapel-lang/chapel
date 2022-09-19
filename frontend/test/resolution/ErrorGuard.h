/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef FRONTEND_TEST_RESOLUTION_ERROR_GUARD_H
#define FRONTEND_TEST_RESOLUTION_ERROR_GUARD_H

#include "chpl/framework/Context.h"
#include "chpl/framework/ErrorMessage.h"
#include <vector>

class ErrorGuard {
 private:
  using BaseHandler = chpl::owned<chpl::Context::ErrorHandler>;
  BaseHandler oldErrorHandler_;
  chpl::Context* ctx_;

  BaseHandler prepareHandler() const {
    bool report = false;
    bool store = true;
    return chpl::toOwned(new chpl::Context::ErrorHandler(report, store));
  }

 public:
  ErrorGuard(chpl::Context* ctx) : ctx_(ctx) {
    oldErrorHandler_ = ctx_->installErrorHandler(prepareHandler());
  }

  inline chpl::Context* context() const { return ctx_; }

  const std::vector<const chpl::ErrorBase*>& errors() const {
    auto handler = ctx_->errorHandler();
    assert(handler);
    return handler->errors();
  }

  bool realizeErrors() {
    auto handler = ctx_->errorHandler();
    assert(handler);
    if (!handler->errors().size()) return false;
    for (auto err : handler->errors()) handler->report(ctx_, err);
    std::ignore = ctx_->installErrorHandler(prepareHandler());
    return true;
  }

  ~ErrorGuard() {
    assert(!this->realizeErrors());
    std::ignore = ctx_->installErrorHandler(std::move(oldErrorHandler_));
  }
};

#endif
