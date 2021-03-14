enum mybool {
  myfalse=0,
  mytrue=1
}

proc foo(b: uint) {
  writeln("In uint foo");
}

config param testenum = mytrue;

var b: bool;
var mb: mybool;

foo(b);
if testenum then
  foo(mb);
