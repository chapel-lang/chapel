// forall-expression

use datas;

proc multiply(arg1: int, arg2: int)  { writeln("mul=", arg1*arg2); }

// error - parallel iterator is not provided
[ idx in (forall IND in inst1ser do IND) ]
  multiply(idx,0);
