use Time;

def main {
  var x: int;
  var z: int;
  cobegin {
    begin {
      x = 2;
      writeln((x, z));
    }
    {
      sleep(2);
      z = x;
    }
  }
  writeln((x, z));
}
