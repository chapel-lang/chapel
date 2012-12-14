interface LessThan(type T) {
  proc LT(a:T, b:T):bool;
}

implements LessThan(int);

proc LT(a:int, b:int) : bool {
  return a < b;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(b = x, a = y)) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(1, 2));

