class InParallel {
  param a: int;
  param b: real;

  proc init(param x: int, param y: real) {
    cobegin { // Uh oh, fields can't be initialized in parallel!
      a = x;
      b = y;
    }
  }
}

proc main() {
  var c: shared InParallel(1, 2.0) = new shared InParallel(1, 2.0);
  writeln(c.type: string);
}
