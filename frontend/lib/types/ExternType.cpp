#include "chpl/types/ExternType.h"
#include "chpl/framework/query-impl.h"

namespace chpl {
namespace types {

const owned<ExternType>& ExternType::getExternType(Context* context,
                                                   UniqueString name) {
  QUERY_BEGIN(getExternType, context, name);
  auto result = toOwned(new ExternType(name));
  return QUERY_END(result);
}

const ExternType* ExternType::get(Context* context, UniqueString linkageName) {
  return getExternType(context, linkageName).get();
}

} // end namespace types
} // end namespace chpl
