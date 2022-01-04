interface LessThan(T) {
  proc LT(x:T, y:T):bool;
}

class TestMe {
  var data : int;
}

implements LessThan(TestMe);

proc LT(x:TestMe, y:TestMe) : bool {
  return x.data < y.data;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  } else {
    return x;
  }
}

var a = new shared TestMe(3);
var b = new shared TestMe(4); 

// Writes 3
writeln(minFn(a, b));

writeln(minFn(a.borrow(), b.borrow()));
