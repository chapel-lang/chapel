class pair {
  var a: int;
  var b: real;
}

var a: unmanaged pair = new unmanaged pair();

proc callin(in x: unmanaged pair) {
  writeln("in callin, x is: ", x.a, " ", x.b);

  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }

  x   = new unmanaged pair();
  x.a = 11;
  x.b = 3.4;

  if (x == a) {
    writeln("a and x are the same on the way out");
  } else {
    writeln("a and x differ on the way out");
  }

  writeln("re-assigned to be new instance: ", x.a, " ", x.b);

  delete x;
}


proc callout(out x: unmanaged pair?) {
  writeln("in callout, x should be nil");

  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }

  x    = new unmanaged pair();
  x!.a = 12;
  x!.b = 4.5;

  if (x == a) {
    writeln("a and x are the same on the way out");
  } else {
    writeln("a and x differ on the way out");
  }

  writeln("re-assigned to be new instance: ", x!.a, " ", x!.b);
}


proc callinout(inout x: unmanaged pair) {
  writeln("in callinout, x is: ", x.a, " ", x.b);

  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }

  x   = new unmanaged pair();
  x.a = 13;
  x.b = 5.6;

  if (x == a) {
    writeln("a and x are the same on the way out");
  } else {
    writeln("a and x differ on the way out");
  }

  writeln("re-assigned to be new instance: ", x.a, " ", x.b);
}


proc callblank(x: unmanaged pair) {
  writeln("in callblank, x is: ", x.a, " ", x.b);

  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }
}


proc main() {
  a.a = 10;
  a.b = 2.3;

  callin(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  {
    var t1 = a;
    var aa: unmanaged pair? = a;
    callout(aa);
    writeln("back at callsite, a is: ", aa!.a, " ", aa!.b);
    writeln();
    delete t1;
    a = aa!;
  }

  var t2 = a;
  callinout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();
  delete t2;

  callblank(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);

  delete a;
}
