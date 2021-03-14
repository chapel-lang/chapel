use iters;
use datas;

proc multiply(arg1: int, arg2: int)  { writeln("mul=", arg1*arg2); }
proc summify(arg1: int, arg2: int)   { writeln("sum=", arg1+arg2); }
proc tuplify(arg1: 2*int, arg2: int) { writeln("tup=", (arg1(1)+arg1(2))*arg2); }

writeln();

// I am using the "serial OK" form for the forall expression
// and the forall statement, to avoid worrying about compiler errors.

writeln("[IND in iter1SA()] IND");
[ idx in ([IND in iter1SA()] IND) ]
  multiply(idx,0);
showCountsI;

writeln("[IND in inst1SA] IND");
[ idx in ([IND in inst1SA] IND) ]
  multiply(idx,0);
showCountsD;
