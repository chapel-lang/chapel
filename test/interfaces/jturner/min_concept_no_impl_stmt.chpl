interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

proc LT(x:int, y:int) : bool {
  return x < y;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(3, 4));

