interface LessThan(T) {
  proc LT(a:T, b:T):bool;
}

implements LessThan(int);

proc LT(a:int, b:int) : bool {
  return a < b;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  // Re-align formals/actuals when installing call target from passed in model
  if (LT(b = x, a = y)) {
    return y;
  } else {
    return x;
  }
}

// Writes 1
writeln(minFn(1, 2));
