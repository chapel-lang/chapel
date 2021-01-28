interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

// Error: LT(int, int) not in scope
implements LessThan(int);

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  } else {
    return x;
  }
}

writeln(minFn(3, 4));
