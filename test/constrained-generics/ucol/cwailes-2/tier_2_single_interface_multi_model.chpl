// UCol original version did not have the Addable interface
// which is now engaged as required to check the + in j().

module Main {

module ILib {
  interface I(T) {
    proc f(x:T):T;
  }
  interface Addable(U,V) {
    operator +(arg1: U, arg2: V): U;
  }
}

module A {
  use super.ILib;

  proc f(x:int):int { return x; }

  // Model 1
  implements I(int);
  
  proc j(x:?U, y:?V)
    where implements I(U) && implements I(V) && implements Addable(U,V)
  {
    return f(x) + f(y);
  }
  
  // A.j is passed [models[0], Model #1]
  proc g(x:?U):U where implements I(U) && implements Addable(U, int) {
    return j(x, 21);
  }
}

module B {
  use super.ILib;
  import super.A;

  proc f(x:int):int { return x + 1; }
  
  // Model 2
  implements I(int);
  implements Addable(int, int);

  proc h(x:int):int {
    // A.g is passed [model #2]
    return A.g(x);
  }
}

// Prints 42
writeln(B.h(20));

}
