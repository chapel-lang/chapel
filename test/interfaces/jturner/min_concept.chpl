interface LessThan {
  proc LT(x:self, y:self):bool;
}

proc minFn(x:?T, y:T):T where T implements LessThan {
  if (LT(y, x)) {
    return y;
  }
  else {
    return x;
  }
}

int implements LessThan;

proc LT(x:int, y:int) : bool {
  return x < y;
}


writeln(minFn(3, 4));

