record R { var x: int; }

var r1 = R(1);

def foo() {
  return r1;
}

def bar() {
  var r2 = R(2);
  return r2;
}

writeln("foo().x = ", foo().x);
writeln("bar().x = ", bar().x);

writeln("trying to write into foo().x and bar().x...");
foo().x = 3;
bar().x = 4;
writeln("succeeded");

writeln("r1 is: ", r1);
writeln("foo().x = ", foo().x);
writeln("bar().x = ", bar().x);
