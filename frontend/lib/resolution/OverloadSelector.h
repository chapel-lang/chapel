#include "chpl/framework/Context.h"
#include "chpl/framework/ID.h"
#include "chpl/uast/BuilderResult.h"

namespace chpl {
namespace resolution {

using BuilderFunc = uast::BuilderResult const&(*)(Context* context, ID typeId, int overloadIdx);

template <BuilderFunc... Fs>
struct OverloadSelector {

  template <int idx, BuilderFunc F, BuilderFunc Head, BuilderFunc ... Tail>
  static std::enable_if_t<F == Head, uast::BuilderResult const&> invokeImpl(Context* context, const ID& id) {
    return F(context, id, idx);
  }

  template <int idx, BuilderFunc F, BuilderFunc Head, BuilderFunc ... Tail>
  static std::enable_if_t<F != Head, uast::BuilderResult const&> invokeImpl(Context* context, const ID& id) {
    return invokeImpl<idx + 1, F, Tail...>(context, id);
  }

  template <BuilderFunc F>
  static uast::BuilderResult const& invoke(Context* context, const ID& id) {
    return invokeImpl<0, F, Fs...>(context, id);
  }

  template <BuilderFunc Head, BuilderFunc ... Tail>
  static uast::BuilderResult const& selectImpl(Context* context, const ID& id, int targetIdx, int currentIdx) {
    if (currentIdx == targetIdx) {
      return Head(context, id, currentIdx);
    } else {
      if constexpr (sizeof...(Tail) == 0) {
        throw std::out_of_range("Index out of range");
      } else {
        return selectImpl<Tail...>(context, id, targetIdx, currentIdx + 1);
      }
    }
  }

  static uast::BuilderResult const& select(Context* context, const ID& id, UniqueString overloadPart) {
    // note: this should match the way doAssignIDs in Builder.cpp creates
    // the overloadPart from the index of the overload.
    int targetIdx = 0;
    if (!overloadPart.isEmpty()) {
      targetIdx = atoi(overloadPart.c_str());
    }

    return selectImpl<Fs...>(context, id, targetIdx, 0);
  }
};

} // namespace resolution
} // namespace chpl
