var i1 = 1;

proc foo() {
  return i1;
}

proc bar() {
  var i2 = 2;
  return i2;
}

writeln("foo() = ", foo());
writeln("bar() = ", bar());

writeln("trying to write into foo() and bar()...");
foo() = 3;
bar() = 4;
writeln("succeeded");

writeln("i1 is: ", i1);
writeln("foo() = ", foo());
writeln("bar() = ", bar());
