var x: uint(64);

proc main() {
  var i, j: int;
  coforall i in 1..10 {
    atomic {
      foo();
      j += 1;
      foo(); // check if the cache works
    }
  }
  writeln("x = ", x, " j = ", j);
}

proc foo() {
  x = x + 1;
}