proc foo.secondary() {
  writeln("secondary method call; i is ", i);
}

class foo {
  var i : int;
  proc primary() {
    writeln("primary method call; i is ", i);
  }
}


var ownF1 = new owned foo();
var f1 : borrowed foo = ownF1.borrow();

f1.primary();
f1.secondary();



var ownF2 = new owned foo();
var f2 : borrowed foo = ownF2.borrow();
f2.i = 4;

f2.primary();
f2.secondary();
