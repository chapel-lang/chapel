interface LessThan(type T) {
  proc T.less_than(y:T):bool;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (y.less_than(x)) {
    return y;
  } else {
    return x;
  }
}

class Foo {
  var n : int;
  proc less_than(y:Foo): bool {
    return n < y.n;
  }
}

implements LessThan(Foo);

// Writes 3
writeln(minFn(new Foo(3), new Foo(4)).n);

