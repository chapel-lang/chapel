// This test is a variant of min_concept_auto_promote.chpl
// allowing promotion of the required function.

interface LessThan(T) {
  proc LT(x:T, y:T, ref result: atomic bool): void;
}

proc LT(x:int, y:int, ref result: atomic bool): void {
  if x < y then result.write(true);
}

proc minFn(x:?T, y:T) : T where implements LessThan(T) {
  var lessThan: atomic bool;
  LT(y, x, lessThan);
  if lessThan.read() {
    return y;
  }
  else {
    return x;
  }
}

implements LessThan( [1..3] int) ;

var a:[1..3] int = (1,2,3);
var b:[1..3] int = (1,0,4);

writeln(minFn(a,b));
