proc foo.secondary() {
  writeln("secondary method call; i is ", i);
}

class foo {
  var i : int;
  proc primary() {
    writeln("primary method call; i is ", i);
  }
}



var f1 : foo = new foo();

f1.primary();
f1.secondary();

delete f1;





var f2 : foo = new foo();
f2.i = 4;

f2.primary();
f2.secondary();

delete f2;
