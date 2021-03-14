var a1: [1..3] int = (1, 2, 3);

proc foo() {
  return a1;
}

proc bar() {
  var a2: [1..3] int = (4, 5, 6);
  return a2;
}

writeln("foo()(1) = ", foo()(1));
writeln("bar()(1) = ", bar()(1));

writeln("trying to write into foo().x and bar().x...");
foo()(1) = 3;
bar()(1) = 4;
writeln("succeeded");

writeln("a1 is: ", a1);
writeln("foo().x = ", foo()(1));
writeln("bar().x = ", bar()(1));
