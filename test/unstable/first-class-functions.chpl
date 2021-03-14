proc foo() {
  writeln("In foo");
}

var f = foo;

f();

var g = lambda() { writeln("In anon"); };

g();
