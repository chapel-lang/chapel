var a = 1.0, b, c: int, d, e : real = 1.0, f: real;
writeln("a = ", a, ", b = ", b, ", c = ", c,
        ", d = ", d, ", e = ", e, ", f = ", f);

if(isReal(a) && isReal(d) && isReal(e) && isReal(f)){
  writeln("real type matches");}

if(isInt(b) && isInt(c)){
  writeln("int type matches");}

