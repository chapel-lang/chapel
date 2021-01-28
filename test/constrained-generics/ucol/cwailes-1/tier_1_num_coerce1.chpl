interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

implements LessThan(real);

proc LT(x:real, y:real) : bool {
  return x < y;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  } else {
    return x;
  }
}

writeln(minFn(3.0, 4));

