interface LessThan {
  proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int) : bool {
  return x < y;
}

proc minFn(x:?T, y:T):T where T implements LessThan {
  if (LT(y, x)) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(3, 4));

