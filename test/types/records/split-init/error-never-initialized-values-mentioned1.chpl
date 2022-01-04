class C { }

proc error() {
  // These have invalid split inits
  var a2;

  writeln(a2);

  a2 = 1;
}
error();
