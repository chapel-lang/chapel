class C { }

proc error() {
  // These have invalid split inits
  var q2: C?;

  writeln(q2);

  q2 = new owned C();
}
error();
