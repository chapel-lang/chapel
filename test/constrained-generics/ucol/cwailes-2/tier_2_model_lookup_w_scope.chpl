module Main {

module ILib {
  interface I(T) {
    proc f(x:T):T;
  }
}

module A {
  use super.ILib;

  proc f(x:int):int { return x; }

  // Model 1
  implements I(int);

  proc g(x:?U):U where implements I(U) {
    return f(x);
  }
}

module B {
  use super.ILib;
  import super.A;

  proc f(x:int):int { return x + 1; }
  
  // Model 2
  implements I(int);

  proc h(x:int): int {
    // A.g is passed model #2
    return A.g(x);
  }
}

// Prints 42
writeln(B.h(41));

}
