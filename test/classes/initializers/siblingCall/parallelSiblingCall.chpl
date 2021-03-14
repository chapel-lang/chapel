class ParaThis {
  var val: int;

  proc init() {
    begin this.init(7); // Uh oh!
    val += 1;
  }

  proc init(start: int) {
    val = start;
  }
}

proc main() {
  var c: borrowed ParaThis = new borrowed ParaThis();
  writeln(c);
}
