interface LessThan {
  proc LT(x:self, y:self):bool;
}

record TestMe {
  var data;
}

implements LessThan(TestMe);

proc LT(x:TestMe, y:TestMe) : bool {
  return x.data < y.data;
}

proc minFn(x:?T, y:T):T where implements LessThan(T) {
  if (LT(y, x)) {
    return y;
  }
  else {
    return x;
  }
}

var a = new TestMe(3);
var b = new TestMe(4); 

writeln(minFn(a, b));

