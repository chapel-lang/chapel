interface LessThan(T) {
  proc T.less_than(y:T):bool;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (y.less_than(x)) {
    return y;
  }
  else {
    return x;
  }
}

class C {
  var n : int;
  proc less_than(y:C): bool {
    return n < y.n;
  }
}

implements LessThan(C);

writeln(minFn(new C(3), new C(4)).n);

