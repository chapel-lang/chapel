
proc callin(in x: string) {
  writeln("in callin, x is: ", x);
  x += "b";
  writeln("re-assigned to be: ", x);
}


proc callout(out x: string) {
  writeln("in callout, x is: ", x);
  x += "c";
  writeln("re-assigned to be: ", x);
}


proc callinout(inout x: string) {
  writeln("in callinout, x is: ", x);
  x += "d";
  writeln("re-assigned to be: ", x);
}


proc callblank(x: string) {
  writeln("in callblank, x is: ", x);
}


proc main() {
  var a: string = "a";

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
