interface LessThan(type T) {
  proc LT(x:T, y:T):bool;
}

proc LT(x:int, y:int) : bool {
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

implements LessThan( [1..3] int) ;

var a:[1..3] int = (1,2,3);
var b:[1..3] int = (1,2,4);

writeln(minFn(a,b));
