#ifndef CHPL_AST_SYMBOL_H
#define CHPL_AST_SYMBOL_H

#include "chpl/AST/BaseAST.h"
#include "chpl/AST/UniqueString.h"

namespace chpl {
namespace ast {

/**
  This is an abstract base class for Symbols
 */
class Symbol : public BaseAST {

 public:
  enum Visibility {
    VISIBILITY_DEFAULT,
    VISIBILITY_PUBLIC,
    VISIBILITY_PRIVATE,
  };

 private:
  UniqueString name_;
  Visibility visibility_;

 protected:
  Symbol(asttags::ASTTag tag, UniqueString name, Visibility vis);
  Symbol(asttags::ASTTag tag, ASTList children,
         UniqueString name, Visibility vis);
  bool symbolContentsMatchInner(const Symbol* other) const {
    return this->name_ == other->name_ &&
           this->visibility_ == other->visibility_;
  }

 public:
  virtual ~Symbol() = 0; // this is an abstract base class

  UniqueString name() const { return name_; }
  Visibility visibility() const { return visibility_; }
};

} // end namespace ast
} // end namespace chpl

#endif
