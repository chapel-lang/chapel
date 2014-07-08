use Time;

proc main {
  var x: int;
  var z: int;
  var s: sync int;
  cobegin ref(x, z) {
    begin ref(x) {
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
