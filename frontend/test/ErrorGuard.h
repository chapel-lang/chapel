/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef FRONTEND_TEST_ERROR_GUARD_H
#define FRONTEND_TEST_ERROR_GUARD_H

#include "chpl/framework/Context.h"
#include "chpl/framework/ErrorWriter.h"
#include <vector>

using BaseHandler = chpl::Context::ErrorHandler;

class AggregatingErrorHandler : BaseHandler {
  std::vector<chpl::owned<chpl::ErrorBase>> errors_;
 public:
  AggregatingErrorHandler() = default;
  ~AggregatingErrorHandler() = default;

  const std::vector<chpl::owned<chpl::ErrorBase>>& errors() const {
    return errors_;
  }

  template <typename C>
  void moveErrors(C& container) {
    std::move(errors_.begin(), errors_.end(), std::back_inserter(container));
    errors_.clear();
  }

  virtual void
  report(chpl::Context* context, const chpl::ErrorBase* err) override {
    errors_.push_back(err->clone());
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

  /** A way to iterate over the errors contained in the guard. */
  const std::vector<chpl::owned<chpl::ErrorBase>>& errors() const {
    assert(handler_);
    return handler_->errors();
  }

  /** Get the number of errors contained in the guard. */
  inline size_t numErrors(bool countWarnings = true) const {
    size_t ret = handler_->errors().size();
    if (!countWarnings) {
      for (auto& err : handler_->errors())
        if (err->kind() == chpl::ErrorBase::WARNING) --ret;
    }
    return ret;
  }

  const chpl::owned<chpl::ErrorBase>& error(size_t idx) const {
    assert(idx < numErrors());
    return this->errors()[idx];
  }

  void clearErrors() {
    assert(handler_);
    handler_->clear();
  }

  /** Print the errors contained in this guard and then clear the guard
      of errors. Returns the number of errors. */
  int realizeErrors(bool countWarnings = true) {
    assert(handler_);
    if (!handler_->errors().size()) return false;
    this->printErrors();
    int ret = (int) handler_->errors().size();

    if (!countWarnings) {
      for (auto& err : handler_->errors())
        if (err->kind() == chpl::ErrorBase::WARNING) --ret;
    }

    handler_->clear();
    return ret;
  }

  template <typename C>
  void moveErrors(C& container) {
    handler_->moveErrors(container);
  }

  /** Print the errors contained in this guard in a detailed manner. */
  void printErrors() const {
    chpl::ErrorWriter ew(this->context(), std::cout,
                         chpl::ErrorWriter::DETAILED,
                         false);
    for (auto& err : this->errors()) err->write(ew);
    std::cout.flush();
  }

  /** The guard destructor will assert that no errors have occurred. */
  ~ErrorGuard() {
    assert(!this->realizeErrors());
    std::ignore = ctx_->installErrorHandler(std::move(oldErrorHandler_));
  }
};

#endif
