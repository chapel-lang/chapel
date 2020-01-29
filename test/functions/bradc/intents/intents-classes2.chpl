class pair {
  var a: int;
  var b: real;
}

proc callin(in x: unmanaged pair) {
  writeln("in callin, x is: ", x.a, " ", x.b);

  x   = new unmanaged pair();
  x.a = 11;
  x.b = 3.4;

  writeln("re-assigned to be new instance: ", x.a, " ", x.b);

  delete x;
}


proc callout(out x: unmanaged pair?) {
  writeln("in callout, x ought to be nil");

  x    = new unmanaged pair();
  x!.a = 12;
  x!.b = 4.5;

  writeln("re-assigned to be new instance: ", x!.a, " ", x!.b);
}


proc callinout(inout x: unmanaged pair) {
  writeln("in callinout, x is: ", x.a, " ", x.b);

  delete x;

  x   = new unmanaged pair();
  x.a = 13;
  x.b = 5.6;

  writeln("re-assigned to be new instance: ", x.a, " ", x.b);
}


proc callblank(x: unmanaged pair) {
  writeln("in callblank, x is: ", x.a, " ", x.b);

  x.a = 14;
  x.b = 6.7;

  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc main() {
  var a: unmanaged pair = new unmanaged pair();

  a.a = 10;
  a.b = 2.3;

  callin(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  {
    var aa: unmanaged pair?;
    callout(aa);
    writeln("back at callsite, a is: ", aa!.a, " ", aa!.b);
    writeln();

    delete a;
    a = aa!;
  }

  callinout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callblank(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);

  delete a;
}
