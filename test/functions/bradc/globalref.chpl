var a = 10;
var b = 20;
var c = 30;

proc foo(a) {
  var c = 3;
  writeln("In foo:");
  writeln("  a is ", a);
  writeln("  b is ", b);
  writeln("  c is ", c);
  writeln();
}

writeln("In main:");
writeln("  a is ", a);
writeln("  b is ", b);
writeln("  c is ", c);
writeln();

foo(1);

writeln("In main:");
writeln("  a is ", a);
writeln("  b is ", b);
writeln("  c is ", c);
writeln();
