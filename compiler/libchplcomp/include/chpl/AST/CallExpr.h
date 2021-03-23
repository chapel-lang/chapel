#ifndef CHPL_AST_CALLEXPR_H
#define CHPL_AST_CALLEXPR_H

namespace chpl {

// This class represents a call to a function
class CallExpr : final Expr {
 public:
   struct PossiblyNamedActual {
     UniqueString name; // "" if unused
     Expr* actual;
   };

 private:
   // this represents the called expression e.g. 'f' in 'f(1,2,3)'
   Expr* baseExpr_;
   // this represents the actuals e.g. 1,2,b=3 in 'f(1,2,b=3)'
   // TODO: Would it help to use llvm::SmallVector here?
   std::vector<PossiblyNamedActual> actuals;
   // TODO: do we need partialTag / methodTag?
 public:
  ~CallExpr() override = default;

  Expr* baseExpr() const { return baseExpr_; }
  int numActuals() const { return actuals.size(); }
  Expr* actual(int i) const { return actuals[i].actual; }
  bool actualIsNamed(int i) const { return !actuals[i].name.isEmpty(); }
  // Returns the name of the actual, if used; otherwise the empty string
  UniqueString actualName(int i) const { return actuals[i].name; }
};

}
#endif
