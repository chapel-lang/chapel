// forall-expression

use iters;

proc tuplify(arg1: 2*int, arg2: int) { writeln("tup=", (arg1(1)+arg1(2))*arg2); }

// error - leader is not provided
[ idx in (forall IND in zip(iter1SA(), iter2SA()) do IND) ]
  tuplify(idx,0);
