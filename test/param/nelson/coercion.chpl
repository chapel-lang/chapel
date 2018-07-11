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
  var a = new C(0 : uint); // this works
  var b = new C(1);        // this doesn't

  f(2); // this also works

  delete b;
  delete a;
}