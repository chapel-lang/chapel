// UCol original version did not have the Addable interface
// so the `+` within g() did not typecheck.


interface I(T) {
  proc f(x:T):T;
}

interface Addable(U,V) {
  proc +(arg1: U, arg2: V): U;
}

proc g(x:?U, y:?V):U
  where implements I(U) && implements I(V) && implements Addable(U, V)
{
  // models[0][0](x) + model[1][0](x)
  return f(x) + f(y);
}

proc h(x:?T):T where implements I(T) {
  // Calling g from here would be incorrect if we used any other pair of
  // parameters than (x, x) because it would place additional constraints on
  // the types.  You are not allowed to call more generics that are more
  // constrained than the calling generic.  However, in this case we are just
  // duplicating the same constraint.
  return g(x, x);
}

proc f(x:int) { return x * 2; }

implements I(int);
implements Addable(int, int);

// Prints 42
writeln(h(21));
