var z: int;

proc main () {
  var a: int;

  coforall i in 1..1000 do 
    atomic a += foobar();

  writeln("a = ", a, " z = ", z);

  coforall i in 1..1000 {
    var b: int;
    b = foobar();
    atomic a += b;
  }

  writeln("a = ", a, " z = ", z);
}

proc foobar(): int {
  var x: int;
  proc foo() {
    var y: int;
    proc bar() {
      z += 1;
      x += 1;
      y += 2;
    }
    bar();
  }
  atomic foo();
  atomic foo();
  return x;
}
