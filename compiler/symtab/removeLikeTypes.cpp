#include "removeLikeTypes.h"
#include "type.h"
#include "expr.h"

void RemoveLikeTypes::processSymbol(Symbol* sym) {
  if (LikeType* like_type = dynamic_cast<LikeType*>(sym->type)) {
    sym->type = like_type->expr->typeInfo();
  }
}
