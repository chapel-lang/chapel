function foo(inout x: integer): integer {
  x += 100;
  writeln("In foo, x is: ", x);
  return 2*x;
}

function bar(inout x: integer): integer {
  x += 1000;
  writeln("In bar, x is: ", x);
  return foo(x);
}

function main() {
  var a: integer = 10;
  var r: integer;
  r = bar(a);
  writeln("In main, a is: ", a, ", r is: ", r);
}