interface I(type T) {
  proc f(x:T):T;
}

module A {
  proc f(x:int):int { return x; }

  // Model 1
  implements I(int);
  
  proc j(x:?U, y:?V) where implements I(U), I(V) {
    return f(x) + f(y);
  }
  
  // A.j is passed [models[0], Model #1]
  proc g(x:?U):U where implements I(U) {
    return j(x, 21);
  }
}

module B {
  proc f(x:int):int { return x + 1; }
  
  // Model 2
  implements I(int);

  proc h(x:int):int {
    // A.g is passed [model #2]
    return A.g(x);
  }
}

// Prints 42
writeln(B.h(20));
