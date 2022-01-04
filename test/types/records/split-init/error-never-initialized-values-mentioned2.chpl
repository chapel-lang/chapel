class C { }

proc error() {
  // These have invalid split inits
  var b2: borrowed C;

  writeln(b2);

  b2 = (new owned C()).borrow();
}
error();
