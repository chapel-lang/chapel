class Scoping {
  var a = 7;
  var b: int;
  var c: uint;
  var d = -2;

  proc init(aVal, bVal, cVal, dVal) {
    {
      {
        a = aVal;
        {
          b = bVal;
        }
      }
      c = cVal;
    }
    d = dVal;
    super.init();
  }
}

proc main() {
  var s: Scoping = new Scoping(9, 12, 5:uint, -8);
  writeln(s);
  delete s;
}
