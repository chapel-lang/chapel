proc (int(32)).foo() {
  writeln(this);
}

var x:int(32) = 1;
x.foo(); // this should work

var y:int(64) = 1;
y.foo(); // this should not
