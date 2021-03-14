class Scoping {
  var a = 7;
  var b: int;

  proc init(aVal, bVal) {
    {
      a = aVal;
    }
    b = bVal;
  }
}

proc main() {
  var s: unmanaged Scoping = new unmanaged Scoping(9, 12);
  writeln(s);
  delete s;
}
