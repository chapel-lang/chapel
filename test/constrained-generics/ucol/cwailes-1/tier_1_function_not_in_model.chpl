interface LessThan {
  proc LT(x:self, y:self):bool;
}

interface GreaterThan {
  proc GT(x:self, y:self):bool;
}

implements LessThan(int);
implements GreaterThan(int);

proc LT(x : int, y : int) : bool {
  return x < y;
}

proc minFn(x:?T, y:T):T where implements GreaterThan(T) {
  // Error: Can't resolve call to LT(T, T)
  if (LT(y, x)) {
    return y;
  } else {
    return x;
  }
}

writeln(minFn(3, 4));
