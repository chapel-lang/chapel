// The generic return type: only the memory-management kind is declared.

// See also:
//   test/functions/lydia/declaredGenericReturnType.chpl
//   test/users/thom/returnGeneric.chpl

const globO = new owned object();

/// proc+return ///

proc retOw(): owned      { return new owned object(); }
proc retSh(): shared     { return new shared object(); }
proc retBw(): borrowed   { return globO; } // implicit borrow
proc retUm(): unmanaged  { return new unmanaged object(); }

var owP = retOw();
var shP = retSh();
var bwP = retBw();
var umP = retUm();

compilerWarning(owP.type:string);
compilerWarning(shP.type:string);
compilerWarning(bwP.type:string);
compilerWarning(umP.type:string);

delete umP;

/// iter+yield ///

iter yldOw(): owned      { yield new owned object(); }
iter yldSh(): shared     { yield new shared object(); }
iter yldBw(): borrowed   { yield globO; } // implicit borrow
iter yldUm(): unmanaged  { yield new unmanaged object(); }

var owI = yldOw();
var shI = yldSh();
var bwI = yldBw();
var umI = yldUm();

compilerWarning(owI.type:string);
compilerWarning(shI.type:string);
compilerWarning(bwI.type:string);
compilerWarning(umI.type:string);

delete umI;
