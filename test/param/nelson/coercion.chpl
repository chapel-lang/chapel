class C {
  param i : uint;

  proc postinit() {
    writeln("hello from id ", i, "!");
  }
}

proc f(param i : uint) {
  writeln("hello from id ", i, "!");
}

proc main() {
  var ownA = new owned C(0 : uint); // this works
  var a = ownA.borrow();
  var ownB = new owned C(1); // this doesn't
  var b = ownB.borrow();

  f(2); // this also works
}
