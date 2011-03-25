var z: int;

proc main () {
  var a: int;

  // move a to stack
  coforall i in 1..1000 do 
    atomic a += foobar();

  writeln("a = ", a, " z = ", z);
}

proc foobar(): int {
  // x must stay on the stack
  var x: int;
  proc foo() {
    // y must stay on the stack
    var y: int;
    proc bar() {
      z += 1;
      x += 1;
      y += 2;
    }
    bar();
  }
  foo();
  foo();
  return x;
}
