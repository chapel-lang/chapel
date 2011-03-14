var x: uint(64);

proc main() {
  bar();
}

proc bar() {
  var i, j: int;
  coforall i in 1..10 {
    atomic {
      x = x + 1;
      foo();
      j += 1;
    }
  }
}

proc foo() {
  x = x + 1;
}