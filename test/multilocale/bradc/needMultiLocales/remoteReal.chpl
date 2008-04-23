var s1: real;
var flag1: sync bool;

def main() {
  s1 = 1.0;
  on Locales(1) do begin {
    const tmp = flag1;
    _debugWriteln("s1 is: ", s1);
  }
  s1 = 2.0;
  flag1 = true;

  var s2: real;
  var flag2: sync bool;

  s2 = 1.0;
  on Locales(1) do begin {
    const tmp = flag2;
    _debugWriteln("s2 is: ", s2);
  }
  s2 = 2.0;
  flag2 = true;
}
