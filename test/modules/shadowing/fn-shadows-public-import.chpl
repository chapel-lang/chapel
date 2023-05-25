// test from issue #19160
module M {
  public import N.f;

  proc f() {
    writeln("M.f");
  }

  proc main() {
    f();
  }
}

module N {
  proc f() {
    writeln("N.f");
  }
}
