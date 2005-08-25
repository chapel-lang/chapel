
function callin(in x: integer = 100) {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function callout(out x: integer = 200) {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function callinout(inout x: integer = 300) {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


function callblank(x: integer = 400) {
  writeln("in callblank, x is: ", x);
}


function main() {
  var a: integer = 10;

  callin();
  writeln("back at callsite, a is: ", a);
  writeln();

  callout();
  writeln("back at callsite, a is: ", a);
  writeln();

  callinout();
  writeln("back at callsite, a is: ", a);
  writeln();

  callblank();
  writeln("back at callsite, a is: ", a);
}
