// Test various ways we can violate constness with records and tuples.

config var g = 2;
const TUP = (1,2,3);

record RRR {
  var varfield1: (int,real);
  var varfield2: 3*int;
  const cstfield1: (int,real);
  const cstfield2: 3*int;
  proc RRR() {
    varfield1(1) = 1111;
    varfield2(2) = 1111;
    varfield2(g) = 1111;
    cstfield1(1) = 1111;
    cstfield2(2) = 1111;
    cstfield2(g) = 1111;
  }
}

var   rvar: RRR;
const rconst: RRR;

proc main {
  TUP(g) = 6666;
  TUP(2) = 6666;
  rvar.varfield1(1) = 1111;
  rvar.varfield2(2) = 1111;
  rvar.varfield2(g) = 1111;
  rvar.cstfield1(1) = 6666;
  rvar.cstfield2(2) = 6666;
  rvar.cstfield2(g) = 6666;
  rconst.varfield1(1) = 6666;
  rconst.varfield2(2) = 6666;
  rconst.varfield2(g) = 6666;
  rconst.cstfield1(1) = 6666;
  rconst.cstfield2(2) = 6666;
  rconst.cstfield2(g) = 6666;
  writeln(TUP,rvar,rconst);
}
