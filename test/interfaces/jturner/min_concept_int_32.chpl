interface LessThan {
  proc LT(x:self, y:self):bool;
}

int(32) implements LessThan;

proc LT(x:int(32), y:int(32)) {
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

var a:int(32) = 3;
var b:int(32) = 4;
writeln(minFn(a,b));
