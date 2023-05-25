class Scoping {
  param a = 7;
  param b: int;
  param c: uint;
  param d = -2;

  proc init(param aVal, param bVal, param cVal, param dVal) {
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
  }
}

proc main() {
  var s: borrowed Scoping(9, 12, 5:uint, -8)
       = (new owned Scoping(9, 12, 5:uint, -8)).borrow();
  writeln(s.type: string);
}
