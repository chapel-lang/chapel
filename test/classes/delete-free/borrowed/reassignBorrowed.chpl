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
bc!.f(); // OK. bc initializer will live to end of scope

bc = (new owned D()).borrow();
bc!.f(); // Not OK. bc is nil. It was freed at the end of the previous statement
