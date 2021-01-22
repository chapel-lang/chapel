
proc foo(out a) {
  a = 1; // causes a to have inferred type int
  writeln("foo(out a)");
}

proc main() {
  var x;
  foo(x); // split-init setting x's type to int
}
