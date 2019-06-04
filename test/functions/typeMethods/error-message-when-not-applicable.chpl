class C {
  proc type typeMethod() /* where this == C */ {
    writeln("C.typeMethod()");
  }
}

int.typeMethod();
