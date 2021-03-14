class C {
  const x: int;

  proc init() {
    x = 5;
  }
}

var c = new shared C();

writeln(c.x);
