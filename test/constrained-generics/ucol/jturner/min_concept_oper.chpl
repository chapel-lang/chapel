interface LessThan(T) {
  proc <(x:T, y:T):bool;
}

implements LessThan(int);

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (y < x) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(5, 4));

