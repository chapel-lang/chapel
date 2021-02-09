interface LessThan(T) {
  proc LT(x:T, y:T):bool;
}

implements LessThan(int);

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

writeln(minFn(1, 2));
writeln(minFn(3, 4));

