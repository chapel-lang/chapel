class C {
  var x:int;
  proc deinit() {
    writeln("Destroying C x=", x);
  }
}

proc takeOwnership(const in arg:owned C) {
  writeln("in takeOwnership with arg=", arg.borrow());
}


proc make() {
  var x = new owned C(1);
  return x;
}


proc foo() {
  var x = make();

  takeOwnership(x);
}


foo();

