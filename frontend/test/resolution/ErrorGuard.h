/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
#include "chpl/framework/ErrorWriter.h"
#include <vector>

using BaseHandler = chpl::Context::ErrorHandler;

class AggregatingErrorHandler : BaseHandler {
  std::vector<const chpl::ErrorBase*> errors_;
 public:
  AggregatingErrorHandler() = default;
  ~AggregatingErrorHandler() = default;

  const std::vector<const chpl::ErrorBase*>& errors() const {
    return errors_;
  }

  virtual void
  report(chpl::Context* context, const chpl::ErrorBase* err) override {
    errors_.push_back(err);
  }

  inline void clear() { errors_.clear(); }
};

class ErrorGuard {
 private:
  chpl::owned<BaseHandler> oldErrorHandler_;
  chpl::Context* ctx_;
  AggregatingErrorHandler* handler_ = nullptr;

  chpl::owned<BaseHandler> prepareAndStoreHandler() {
    handler_ = new AggregatingErrorHandler();
    auto ret = chpl::toOwned((BaseHandler*) handler_);
    assert(handler_);
    return ret;
  }

 public:
  ErrorGuard(chpl::Context* ctx) : ctx_(ctx) {
    auto handler = prepareAndStoreHandler();
    oldErrorHandler_ = ctx_->installErrorHandler(std::move(handler));
  }

  inline chpl::Context* context() const { return ctx_; }

  const std::vector<const chpl::ErrorBase*>& errors() const {
    assert(handler_);
    return handler_->errors();
  }

  int realizeErrors() {
    assert(handler_);
    if (!handler_->errors().size()) return false;
    chpl::ErrorWriter ew(ctx_, std::cerr, chpl::ErrorWriter::BRIEF, false);
    for (auto err : handler_->errors()) err->write(ew);
    int ret = (int) handler_->errors().size();
    handler_->clear();
    return ret;
  }

  ~ErrorGuard() {
    assert(!this->realizeErrors());
    std::ignore = ctx_->installErrorHandler(std::move(oldErrorHandler_));
  }
};

#endif
