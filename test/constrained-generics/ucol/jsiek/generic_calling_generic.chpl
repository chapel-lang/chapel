interface I(T) {
  proc f(x:T):T;
}

proc g(x:?U):U where implements I(U) {
  return f(x);
}

proc h(x:?Z):Z where implements I(Z) {
  return g(x);
}

proc f(x:int) { return x * 2; }

implements I(int);

writeln(h(21));