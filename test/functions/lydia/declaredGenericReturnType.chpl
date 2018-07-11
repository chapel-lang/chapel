class Gen {
  type t;
}

proc retsGen(type t): Gen {
  return new unmanaged Gen(t);
}

var x = retsGen(int);
var y = retsGen(real);
writeln(x.type: string);
writeln(y.type: string);
delete x, y;
