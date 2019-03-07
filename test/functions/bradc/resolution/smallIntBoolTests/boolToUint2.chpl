enum mybool {
  false=0,
  true=1
}

proc foo(b: uint) {
  writeln("In uint foo");
}

config param testenum = true;

var b: bool;
var mb: mybool;

foo(b);
if testenum then
  foo(mb);
