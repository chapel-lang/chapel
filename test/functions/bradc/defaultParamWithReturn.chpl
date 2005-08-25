
function callin(in x: integer = 100): integer {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
  return x;
}


function callout(out x: integer = 200): integer {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
  return x;
}


function callinout(inout x: integer = 300): integer {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
  return x;
}


function callblank(x: integer = 400): integer {
  writeln("in callblank, x is: ", x);
  var y = x + 1;
  writeln("y assigned to be: ", y);
  return y;
}


function main() {
  var a: integer = 10;
  var b: integer;

  b = callin();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
  writeln();

  b = callout();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
  writeln();

  b = callinout();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
  writeln();

  b = callblank();
  writeln("back at callsite, a is: ", a);
  writeln("back at callsite, b is: ", b);
}
