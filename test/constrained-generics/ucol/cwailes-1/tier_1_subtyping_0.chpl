class C {
  var data:int;
}

class D : C {}

interface I(type T) {
  proc g(x:T):int;
}

proc g(x:C):int {
  return x.data;
}

proc f(x:?T):int where implements I(T) {
  return g(x);
}

// Prints 42
writeln(f(new D(42)));
