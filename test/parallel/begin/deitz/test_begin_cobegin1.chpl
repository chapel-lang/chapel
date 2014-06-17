use Time;

proc main {
  var x: int;
  var z: int;
  var s: sync int;
  cobegin {
    begin {
      x = 2;
      writeln((x, z));
      s = 1;
    }
    {
      s;
      z = x;
    }
  }
  writeln((x, z));
}
