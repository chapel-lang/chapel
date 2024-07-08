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
  var ownS = new owned Scoping(9, 12);
  var s: borrowed Scoping(9, 12) = ownS.borrow();
  writeln(s.type: string);
}
