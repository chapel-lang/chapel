use datas;

proc tuplify(arg1: 2*int, arg2: int)  { writeln("tup=", (arg1(0)+arg1(1))*arg2); }

tuplify(forall IND in zip(inst1LF,  inst2LF) do IND, 1);
