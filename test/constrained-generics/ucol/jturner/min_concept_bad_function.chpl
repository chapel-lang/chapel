interface LessThan {
  proc LT(x:Self, y:Self):bool;
}

interface GreaterThan {
  proc GT(x:Self, y:Self):bool;
}

implements LessThan(int);

proc LT(x : int, y : int) : bool {
  return x < y;
}

proc minFn(x:?T, y:T):T where implements GreaterThan(T) {
  if (GT(x,y)) {
    return y;
  }
  else {
    return x;
  }
}

// Error: int does not implement GreaterThan
writeln(minFn(3, 4));
