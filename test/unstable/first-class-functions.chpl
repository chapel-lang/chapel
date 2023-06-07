proc foo() {
  writeln("In foo");
}

var f = foo;

f();

var g = proc() { writeln("In anon"); };

g();
