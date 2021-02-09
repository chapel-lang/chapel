interface LessThan(T) {
  proc LT(x:T, y:T):bool;
}

interface Equal(T) {
  proc EQ(x:T, y:T):bool;
}

proc LT(x:int, y:int) {
  return x < y;
}

proc EQ(x:int, y:int) {
  return x == y;
}

proc minFn(x:?T, y:T, ifeq:T)
  where implements LessThan(T) && implements Equal(T)
{
  if (LT(y, x)) {
    return y;
  } else if (EQ(x,y)) {
    return ifeq;
  } else {
    return x;
  }
}

implements LessThan(int);
implements Equal(int);

writeln(minFn(3, 4, 5));
writeln(minFn(4, 3, 5));
writeln(minFn(3, 3, 5));
