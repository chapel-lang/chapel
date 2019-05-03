// forall-expression

use iters;

proc multiply(arg1: int, arg2: int)  { writeln("mul=", arg1*arg2); }

// error - parallel iterator is not provided
[ idx in (forall IND in iter1ser() do IND) ]
  multiply(idx,0);
