interface I(type T) {
  proc f(x:T):T;
}

proc g(x:?U):U where I(U) {
  return f(x);
}

proc h(x:?Z):Z where I(Z) {
  return g(x);
}

proc f(x:int) { return x * 2; }

implements I(int);

writeln(h(21));