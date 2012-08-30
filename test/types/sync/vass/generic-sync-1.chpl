
var d: sync int;

test(d);

proc test(s) {
  compilerWarning(typeToString(s.type), 0);
  s = 1;
  writeln("got lock");
  s;
}
