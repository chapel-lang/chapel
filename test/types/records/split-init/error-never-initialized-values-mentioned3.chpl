class C { }

proc error() {
  // These have invalid split inits
  var c2: C;

  writeln(c2);

  c2 = new owned C();
}
error();
