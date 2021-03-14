class Scoping {
  param a = 7;
  param b: int;

  proc init(param aVal, param bVal) {
    {
      a = aVal;
    }
    b = bVal;
  }
}

proc main() {
  var s: borrowed Scoping(9, 12) = new borrowed Scoping(9, 12);
  writeln(s.type: string);
}
