interface LessThanEqual {
  proc LT(x:self, y:self):bool;
  proc EQ(x:self, y:self):bool;
}

proc LT(x:int, y:int) {
  return x < y;
}

proc EQ(x:int, y:int) {
  return x == y;
}

proc minFn(x:?T, y:T, ifeq:T) where T implements LessThanEqual {
  if (LT(y, x)) {
    return y;
  }
  else if (EQ(x,y)) {
    return ifeq;
  }
  else {
    return x;
  }
}

int implements LessThanEqual;

writeln(minFn(3, 4, 5));
writeln(minFn(4, 3, 5));
writeln(minFn(3, 3, 5));
