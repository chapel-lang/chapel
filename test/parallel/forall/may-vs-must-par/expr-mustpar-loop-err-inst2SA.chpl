// forall-expression

use datas;

proc tuplify(arg1: 2*int, arg2: int) { writeln("tup=", (arg1(0)+arg1(1))*arg2); }

// error - leader is not provided
[ idx in (forall IND in zip(inst1SA.borrow(), inst2SA.borrow()) do IND) ]
  tuplify(idx,0);
