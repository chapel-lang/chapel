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
  var a = (new owned C(0 : uint)).borrow(); // this works
  var b = (new owned C(1)).borrow();        // this doesn't

  f(2); // this also works
}
