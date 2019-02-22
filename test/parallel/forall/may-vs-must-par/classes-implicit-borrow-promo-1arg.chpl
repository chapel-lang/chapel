use datas;

proc multiply(arg1: int,  arg2: int)  { writeln("mul=", arg1*arg2); }
proc summify(arg1: int,   arg2: int)  { writeln("sum=", arg1+arg2); }
proc tuplify(arg1: 2*int, arg2: int)  { writeln("tup=", (arg1(1)+arg1(2))*arg2); }

multiply(inst1SA, 0); // bug: the compiler ignores the standalone iterator
