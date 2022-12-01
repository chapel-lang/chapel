proc runQueries(description, A) {
  writeln("Given a ", description, ": ");
  if isDomain(A) {
    writeln("  isRectangularDom() = ", isRectangularDom(A));
    writeln("  isIrregularDom()   = ", isIrregularDom(A));
    writeln("  isAssociativeDom() = ", isAssociativeDom(A));
    writeln("  isSparseDom()      = ", isSparseDom(A));
  }
  if isArray(A) {
    writeln("  isRectangularArr() = ", isRectangularArr(A));
    writeln("  isIrregularArr()   = ", isIrregularArr(A));
    writeln("  isAssociativeArr() = ", isAssociativeArr(A));
    writeln("  isSparseArr()      = ", isSparseArr(A));
  }
}

// empty space needed to match line numbers in .good















// end empty space

// isFloat* functions are being deprecatd
// (in favor of doing isReal(x) || isImag(x) instead)

writeln("isFloat(1.2) returns ", isFloat(1.2));
writeln("isFloat(real) returns ", isFloat(real));
writeln("isFloatType(real) returns ", isFloatType(real));
writeln("isFloatValue(1.2) returns ", isFloatValue(1.2));

// Type subtype relations (use named functions instead of operators).

writeln("Compare 1.type op int");
writeln("op <= returns ", 1.type <= int);
writeln("op <  returns ", 1.type <  int);
writeln("op >= returns ", 1.type >= int);
writeln("op >  returns ", 1.type >  int);

class BaseClass {}
class DerivedClass : BaseClass {}

writeln("Compare BaseClass op DerivedClass");
writeln("op <= returns ", BaseClass <= DerivedClass);
writeln("op <  returns ", BaseClass <  DerivedClass);
writeln("op >= returns ", BaseClass >= DerivedClass);
writeln("op >  returns ", BaseClass >  DerivedClass);

writeln("Compare DerivedClass.type op BaseClass.type");
writeln("op <= returns ", BaseClass <= DerivedClass);
writeln("op <  returns ", BaseClass <  DerivedClass);
writeln("op >= returns ", BaseClass >= DerivedClass);
writeln("op >  returns ", BaseClass >  DerivedClass);
