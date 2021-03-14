proc param int.foo() {
  writeln("In foo() with (nonsensical) param this");
}

proc int.foo() {
  writeln("In foo() with non-param this");
}

2.foo("hi");

