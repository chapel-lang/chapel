class C {
  proc f() {
    writeln("Called C.f()");
  }
}

class D: C {
  override proc f() {
    writeln("Called D.f()");
  }
}

var bc: borrowed C?;
bc = (new owned D()).borrow();
bc!.f(); // Not OK. bc is nil. It was freed at the end of the previous statement
