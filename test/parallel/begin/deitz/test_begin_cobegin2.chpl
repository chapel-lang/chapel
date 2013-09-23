use Time;

proc main {
  var x: int;
  var z: int;
  begin ref(x, z) {
    x = 2;
    var y: int;
    cobegin ref(y,z) {
      y = x + 1;
      z = x + 1;
    }
    writeln((x, y, z));
  }
  sleep(2);
  writeln((x, z));
}
