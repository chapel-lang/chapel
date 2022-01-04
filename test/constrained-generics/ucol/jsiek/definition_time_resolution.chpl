module IFC {
   interface I(T) {
     proc f(x:T):T;
   }
}

module A {
   use IFC;

   proc f(x:int):int { return x; }

   proc g(x:?U):U where implements I(U) {
     return f(x);
   }

}

module B {
   use IFC;
   import A;

   proc f(x:int):int { return x + 1; }

   implements I(int);

   proc h(x:int): int {
     return A.g(x);
   }

}

module Main {
   import B;

   proc main {
     writeln(B.h(41));
   }
}
