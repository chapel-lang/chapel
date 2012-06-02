interface LessThan(type T) {
  proc <(x:T, y:T):bool;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (y < x) {
    return y;
  }
  else {
    return x;
  }
}

writeln(minFn(3, 4));

