var x: uint(64);

proc main() {
  var i: int;
  coforall i in 1..10 {
    atomic {
      x = x + 1;
      foo();
    }
  }  
  writeln("x = ", x);
}

proc foo() {
  x = x + 1;
}