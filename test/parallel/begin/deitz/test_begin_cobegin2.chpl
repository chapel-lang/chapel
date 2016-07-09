proc main {
  var x, z: int;
  sync {
    begin with (ref x, ref z) {
      x = 2;
      var y: int;
      cobegin with (ref y, ref z) {
        y = x + 1;
        z = x + 1;
      }
      writeln((x, y, z));
    }
  }
  writeln((x, z));
}

