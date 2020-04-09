proc foo.secondary() {
  writeln("secondary method call; i is ", i);
}

class foo {
  var i : int;
  proc primary() {
    writeln("primary method call; i is ", i);
  }
}



var f1 : borrowed foo = new borrowed foo();

f1.primary();
f1.secondary();




var f2 : borrowed foo = new borrowed foo();
f2.i = 4;

f2.primary();
f2.secondary();
