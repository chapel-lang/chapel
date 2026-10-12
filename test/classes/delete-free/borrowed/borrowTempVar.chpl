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

var bc: borrowed C? = (new owned C()).borrow();
bc!.f(); // not ok, temporary 'C' is deallocated after line 13.
