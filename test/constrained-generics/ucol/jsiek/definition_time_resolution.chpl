interface I(type T) {
  proc f(x:T):T;
}

module A {

   proc f(x:int):int { return x; }

   proc g(x:?U):U where implements I(U) {
     return f(x);
   }

}

module B {

   proc f(x:int):int { return x + 1; }

   implements I(int);

   proc h(x:int): int {
     return A.g(x);
   }

}

writeln(B.h(41));