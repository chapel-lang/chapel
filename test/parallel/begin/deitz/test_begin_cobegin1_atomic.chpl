proc main {
  var x: int;
  var z: int;
  var s: atomic int;
  cobegin with (ref x, ref z) {
    begin with (ref x) {
      x = 2;
      writeln((x, z));
      s.write(1);
    }
    {
      on Locales[0] {
        s.waitFor(1);
        z = x;
      }
    }
  }
  writeln((x, z));
}
