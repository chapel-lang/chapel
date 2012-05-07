interface LessThan {
  proc LT(x:self, y:self):bool;
}

real implements LessThan;

proc LT(x:real, y:real) : bool {
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

writeln(minFn(3.0, 4));

