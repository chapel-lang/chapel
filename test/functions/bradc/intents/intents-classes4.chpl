
class pair {
  var a: int;
  var b: real;
}

var a: pair = new pair();

proc callin(in x: pair) {
  writeln("in callin, x is: ", x.a, " ", x.b);
  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }
  x = new pair();
  x.a = 11;
  x.b = 3.4;
  if (x == a) {
    writeln("a and x are the same on the way out");
  } else {
    writeln("a and x differ on the way out");
  }
  writeln("re-assigned to be new instance: ", x.a, " ", x.b);
}


proc callout(out x: pair) {
  if (x == nil) {
    writeln("x is nil on the way in as it should be");
  } else {
    writeln("x is not nil!!!");
  }
  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }
  x = new pair();
  x.a = 12;
  x.b = 4.5;
  if (x == a) {
    writeln("a and x are the same on the way out");
  } else {
    writeln("a and x differ on the way out");
  }
  writeln("re-assigned to be new instance: ", x.a, " ", x.b);
}


proc callinout(inout x: pair) {
  writeln("in callinout, x is: ", x.a, " ", x.b);
  if (x == a) {
    writeln("a and x are the same on the way in");
  } else {
    writeln("a and x differ on the way in");
  }
  x = new pair();
  x.a = 13;
  x.b = 5.6;
  if (x == a) {
    writeln("a and x are the same on the way out");
  } else {
    writeln("a and x differ on the way out");
  }
  writeln("re-assigned to be new instance: ", x.a, " ", x.b);
}


proc callblank(x: pair) {
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

  callout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callinout(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
  writeln();

  callblank(a);
  writeln("back at callsite, a is: ", a.a, " ", a.b);
}
