
function callin(in x: integer) {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function callout(out x: integer) {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function callinout(inout x: integer) {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function callblank(x: integer) {
  writeln("in callblank, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function main() {
  var a: integer = 10;

  callin(a);
  writeln("back at callsite, a is: ", a);
  writeln();

  callout(a);
  writeln("back at callsite, a is: ", a);
  writeln();

  callinout(a);
  writeln("back at callsite, a is: ", a);
  writeln();

  callblank(a);
  writeln("back at callsite, a is: ", a);
}
