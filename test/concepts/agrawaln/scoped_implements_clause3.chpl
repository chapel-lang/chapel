interface LessThan {
  proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int) : bool checked {
  return x < y;
}

proc min(type T, x:T, y:T) : T where T implements LessThan checked {
  if (LT(y, x)) {
    return y;
  } else {
    return x;
  }
}

proc test1() checked {
  int implements LessThan;
}

proc test() : int checked {
  return min(int, 3, 4);
}

writeln(test());
