class InParallel {
  var a: int;
  var b: real;

  proc init(x: int, y: real) {
    cobegin { // Uh oh, fields can't be initialized in parallel!
      a = x;
      b = y;
    }
  }
}

proc main() {
  var c: borrowed InParallel = new borrowed InParallel(1, 2.0);
  writeln(c);
}
