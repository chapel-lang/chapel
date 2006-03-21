
fun callin(in x: int = 100) {
  writeln("in callin, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


fun callout(out x: int = 200) {
  writeln("in callout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


fun callinout(inout x: int = 300) {
  writeln("in callinout, x is: ", x);
  x += 1;
  writeln("re-assigned to be: ", x);
}


fun callblank(x: int = 400) {
  writeln("in callblank, x is: ", x);
}


fun main() {
  var a: int = 10;

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
