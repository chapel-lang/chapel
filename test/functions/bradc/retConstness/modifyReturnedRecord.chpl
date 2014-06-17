record R { var x: int; }

var r1 = new R(1);

proc foo() {
  return r1;
}

proc bar() {
  var r2 = new R(2);
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
