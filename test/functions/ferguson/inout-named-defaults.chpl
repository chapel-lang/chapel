proc f1(inout x: int) {
  writeln(x);
  x = 12;
}

{
  writeln("f1 a");
  var arg = 1;
  f1(arg);
  writeln(arg);
}

{
  writeln("f1 b");
  var arg = 1;
  f1(x=arg);
  writeln(arg);
}

proc f2(inout x: int = 22) {
  writeln(x);
  x = 12;
}

{
  writeln("f2 a");
  f2();
}

{
  writeln("f2 b");
  var arg = 1;
  f2(x=arg);
  writeln(arg);
}

proc f3(in x: int, inout y: int = x) {
  writeln(x);
  writeln(y);
  x = 0;
}

{
  writeln("f3 a");
  var arg = 1;
  f3(arg);
  writeln(arg);
}

proc f4(inout x: int = 22, inout y: int = 200) {
  writeln(x);
  writeln(y);
  x = 12;
  y = 120;
}

{
  writeln("f4 a");
  f4();
}

{
  writeln("f4 b");
  var argx = 1;
  f4(argx);
  writeln(argx);
}

{
  writeln("f4 c");
  var argx = 1;
  f4(x=argx);
  writeln(argx);
}

{
  writeln("f4 d");
  var argx = 1;
  var argy = 2;
  f4(x=argx, y=argy);
  writeln(argx);
  writeln(argy);
}

{
  var argx = 1;
  var argy = 2;
  writeln("f4 e");
  f4(y=argy, x=argx);
  writeln(argx);
  writeln(argy);
}

{
  var argx = 1;
  var argy = 2;
  writeln("f4 f");
  f4(argx, argy);
  writeln(argx);
  writeln(argy);
}

{
  var argy = 2;
  writeln("f4 g");
  f4(y=argy);
  writeln(argy);
}
