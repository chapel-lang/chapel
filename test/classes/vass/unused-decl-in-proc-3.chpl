module M {
  use N;
  proc main() {
    writeln("hello");
    if false then delete r();
  }
}

module N {
  proc p {
    class C1 {}
  }
  proc q {
    class C2 {}
    return new unmanaged C2();
  }
  proc r() {
    class C3 {}
    return new unmanaged C3();
  }
}
