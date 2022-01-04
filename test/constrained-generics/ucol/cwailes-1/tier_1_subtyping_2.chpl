/*
This is a variation of tier_1_subtyping_0.chpl
changing the desired implements statement to be for an owned type
instead of a borrowed type.
*/

class C {
  var data:int;
}

class D : C {}

interface I(T) {
  proc g(x:T):int;
}

proc g(x:C):int {
  return x.data;
}

proc f(x:?T):int where implements I(T) {
  return g(x);
}

implements I(owned D);
implements I(shared D);

// Prints 42
writeln(f(new D(42)));
writeln(f(new shared D(42)));
