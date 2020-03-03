class pair {
  var a: int;
  var b: real;
}

proc callin_borrowed(in x: borrowed pair) {
  writeln("in callin borrowed, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}

proc callin_unmanaged(in x: unmanaged pair) {
  writeln("in callin unmanaged, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc callout(out x: unmanaged pair?) {
  writeln("in callout, x ought to be nil");
  assert(x == nil);
  x = new unmanaged pair();
  x!.a = 12;
  x!.b = 4.5;
  writeln("re-assigned new instance to be: ", x!.a, " ", x!.b);
}


proc callinout(inout x: unmanaged pair) {
  writeln("in callinout, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc callblank_borrowed(x: borrowed pair) {
  writeln("in callblank borrowed, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}

proc callblank_unmanaged(x: unmanaged pair) {
  writeln("in callblank unmanaged, x is: ", x.a, " ", x.b);
  x.a += 1;
  x.b += 1.1;
  writeln("re-assigned to be: ", x.a, " ", x.b);
}


proc main() {
  var a = new unmanaged pair();

  a.a = 10;
  a.b = 2.3;

  callin_borrowed(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callin_unmanaged(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  {
    var aa: unmanaged pair? = a;
    callout(aa);
    writeln("back at callsite, a is: ", aa!.a, " ", aa!.b);
    writeln();

    delete a;
    a = aa!;
  }

  callinout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callblank_borrowed(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callblank_unmanaged(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);

  delete a;
}
