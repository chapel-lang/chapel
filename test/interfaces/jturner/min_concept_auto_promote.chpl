interface LessThan {
  proc LT(x:self, y:self):bool;
}

proc LT(x:int, y:int) : bool {
  return x < y;
}

proc minFn(x:?T, y:T) : T where T implements LessThan {
  if (LT(y,x)) {
    return y;
  }
  else {
    return x;
  }
}

[1..3] int implements LessThan;

var a:[1..3] int = (1,2,3);
var b:[1..3] int = (1,2,4);

writeln(minFn(a,b));
