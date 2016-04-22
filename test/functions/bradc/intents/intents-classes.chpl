class pair {
  var a: int;
  var b: real;
}

proc callin(in x: pair) {
  writeln("in callin, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc callout(out x: pair) {
  writeln("in callout, x ought to be nil");
  x = new pair();
  x.a = 12;
  x.b = 4.5;
  writeln("re-assigned new instance to be: ", x.a, " ", x.b);
}


proc callinout(inout x: pair) {
  writeln("in callinout, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc callblank(x: pair) {
  writeln("in callblank, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc main() {
  var a: pair = new pair();

  a.a = 10;
  a.b = 2.3;

  callin(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  delete a;

  callout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callinout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callblank(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);

  delete a;
}
