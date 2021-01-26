interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  } else {
    return x;
  }
}

implements LessThan(int);

proc LT(x:int, y:int) : bool {
  return x < y;
}

// Writes 3
writeln(minFn(3, 4));
