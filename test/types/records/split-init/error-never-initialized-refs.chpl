class C { }

proc error() {
  // These are just never mentioned again
  ref r1;
  
  // These have invalid split inits
  ref r2;

  writeln(r2);

  var a2: int;
  r2 = a2;
}
error();

ref rr;
