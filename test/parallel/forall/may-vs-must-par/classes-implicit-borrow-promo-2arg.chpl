use datas;

proc multiply(arg1: int,  arg2: int)  { writeln("mul=", arg1*arg2); }
proc summify(arg1: int,   arg2: int)  { writeln("sum=", arg1+arg2); }
proc tuplify(arg1: 2*int, arg2: int)  { writeln("tup=", (arg1(0)+arg1(1))*arg2); }

summify(inst1LF, inst2LF);

// combination of the above and zippering
tuplify(forall IND in zip(inst1LF,  inst2LF) do IND, 1);
