var z: int;

proc main () {

  coforall i in 1..1000 do
    atomic foobar();

  foobar();

  writeln("z = ", z);
}

proc foobar() {
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
}
