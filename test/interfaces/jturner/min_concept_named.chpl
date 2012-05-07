interface LessThan {
  proc LT(a:self, b:self):bool;
}

int implements LessThan;

proc LT(a:int, b:int) : bool {
  return a < b;
}

proc minFn(x:?T, y:T):T where T implements LessThan {
  if (LT(b = x, a = y)) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(1, 2));

