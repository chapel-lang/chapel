class C1 {
  proc type typeMethod() /* where this == C1 */ {
    writeln("C1.typeMethod()");
  }
}

class C2 {
  proc type typeMethod() /* where this == C2 */ {
    writeln("C2.typeMethod()");
  }
}

C1.typeMethod();
C2.typeMethod();
