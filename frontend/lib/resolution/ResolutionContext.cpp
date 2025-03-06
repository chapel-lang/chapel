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

#include "chpl/resolution/ResolutionContext.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/types/PlaceholderType.h"
#include "Resolver.h"

namespace chpl {
namespace resolution {

const ID ResolutionContext::Frame::EMPTY_AST_ID = ID();

Resolver*
ResolutionContext::findParentResolverFor(const TypedFnSignature* sig) {
  if (!sig || !sig->parentFn()) return nullptr;
  auto fptr = findFrameMatchingMutable([=](auto& f) {
    return !f.isEmpty() && f.isUnstable() &&
            f.signature() == sig->parentFn() &&
            f.rv() != nullptr;
  });
  return fptr ? fptr->rv() : nullptr;
}

bool ResolutionContext::
canUseGlobalCache(Context* context, const UntypedFnSignature& t) {
  return canUseGlobalCache(context, t.id());
}

bool ResolutionContext::
canUseGlobalCache(Context* context, const TypedFnSignature& t) {
  return !t.isNestedFunction();
}

bool ResolutionContext::
canUseGlobalCache(Context* context, const ID& t) {
  return !parsing::idIsNestedFunction(context, t);
}

bool ResolutionContext::
canUseGlobalCache(Context* context, const MatchingIdsWithName& ids) {
  for (auto& x : ids) if (!canUseGlobalCache(context, x)) return false;
  return true;
}

const ID& ResolutionContext::Frame::id() const {
  if (rv_) {
    if (auto ast = rv_->symbol) return ast->id();
  }
  if (ift_) return ift_->id();
  return EMPTY_AST_ID;
}

const TypedFnSignature* ResolutionContext::Frame::signature() const {
  if (kind_ == ResolutionContext::Frame::FUNCTION && rv_) {
    return rv_->typedSignature;
  }
  return nullptr;
}

static types::QualifiedType placeholderForId(ResolutionContext* rc, const ID& id) {
  return types::QualifiedType(types::QualifiedType::TYPE,
                              types::PlaceholderType::get(rc->context(), id));
}

const types::QualifiedType
ResolutionContext::Frame::typeForContainedId(ResolutionContext* rc, const ID& id) const {
  if (rv_) {
    return rv_->byPostorder.byId(id).type();
  }

  // For interfaces, just return placeholders for the associated types
  // and the interface parameters.
  //
  // In the future, to make generics interoperate with interface-based
  // generics, we will need to do more here. This is pending design discussion,
  // though.
  if (ift_) {
    auto subIt = ift_->substitutions().find(id);
    if (subIt != ift_->substitutions().end())
      return placeholderForId(rc, subIt->first);

    if (witness_) {
      // search associated types
      auto atIt = witness_->associatedTypes().find(id);
      if (atIt != witness_->associatedTypes().end())
        return placeholderForId(rc, atIt->first);

      // TODO: search additional constraints, required functions?
    }
  } else {
    CHPL_ASSERT(witness_ == nullptr);
  }
  return types::QualifiedType();
}

const ResolutionContext::Frame* ResolutionContext::
pushFrame(Resolver* rv, ResolutionContext::Frame::Kind kind) {
  int64_t index = (int64_t) frames_.size();
  frames_.push_back({rv, kind, index});
  auto ret = lastFrame();
  if (ret->isUnstable()) numUnstableFrames_++;
  return ret;
}

const ResolutionContext::Frame* ResolutionContext::
pushFrame(const ResolvedFunction* rf) {
  int64_t index = (int64_t) frames_.size();
  frames_.push_back({rf, index});
  auto ret = lastFrame();
  CHPL_ASSERT(!ret->isUnstable());
  return ret;
}

const ResolutionContext::Frame* ResolutionContext::
pushFrame(const types::InterfaceType* ift, const ImplementationWitness* witness) {
  int64_t index = (int64_t) frames_.size();
  frames_.push_back({ift, witness, index});
  auto ret = lastFrame();
  if (ret->isUnstable()) numUnstableFrames_++;
  return ret;
}

bool ResolutionContext::Frame::isUnstable() const {
  return rv_ != nullptr || ift_ != nullptr || witness_ != nullptr;
}

void ResolutionContext::popFrame(Resolver* rv) {
  CHPL_ASSERT(!frames_.empty() && "Frame stack underflow!");
  CHPL_ASSERT(frames_.back().rv() == rv ||
              frames_.back().ift());

  if (frames_.empty()) return;
  if (frames_.back().isUnstable()) numUnstableFrames_--;
  frames_.pop_back();
}

void ResolutionContext::popFrame(const ResolvedFunction* rf) {
  CHPL_ASSERT(!frames_.empty() && "Frame stack underflow!");
  CHPL_ASSERT(frames_.back().rf() == rf);

  if (frames_.empty()) return;
  CHPL_ASSERT(!frames_.back().isUnstable());
  frames_.pop_back();
}

ResolutionContext createDummyRC(Context* context) {
  return ResolutionContext(context);
}

} // end namespace resolution
} // end namespace chpl
