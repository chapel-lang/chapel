#include "removeLikeTypes.h"
#include "type.h"
#include "expr.h"

void RemoveLikeTypes::processSymbol(Symbol* sym) {
  if (LikeType* like_type = dynamic_cast<LikeType*>(sym->type)) {
    Type* new_type = like_type->expr->typeInfo();
    if (new_type == dtUnknown || new_type == NULL) {
      USR_FATAL(sym, "Cannot resolve type, illegal use of 'like'");
    }
    sym->type = new_type;
  }
}
