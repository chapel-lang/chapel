interface LessThan(T) {
  proc LT(x:T, y:T):bool;
}

implements LessThan(int(32));

proc LT(x:int(32), y:int(32)) {
  return x < y;
}

proc minFn(x:?T, y:T) : T where implements LessThan(T) {
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
