interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

interface Equal(type T) {
  proc EQ(x:T, y:T):bool;
}

proc LT(x:int, y:int) {
  return x < y;
}

proc EQ(x:int, y:int) {
  return x == y;
}

proc minFnPrime(x:?T, y:T, ifeq:T) where implements LessThan(T), Equal(T) {
  if (LT(y, x)) {
    return y;
  } else if (EQ(x,y)) {
    return ifeq;
  } else {
    return x;
  }
}

proc minFn(x:?T, y:T, ifeq:T) where implements LessThan(T) {
  // Error: minFnPrime places more constraints on type T than minFn
  minFnPrime(x, y, ifeq);
}

implements LessThan(int);
implements Equal(int);

writeln(minFn(3,4,5));
