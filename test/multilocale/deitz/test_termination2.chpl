use Time;

def main {
  var s1, s2: sync bool;
  on Locales(1) {
    begin {
      s1;
      on Locales(0) {
        begin {
          s2;
          _debugWriteln("executing on locale ", localeID());
        }
      }
      _debugWriteln("executing on locale ", localeID());
      s2 = true;
    }
  }
  _debugWriteln("executing on locale ", localeID());
  s1 = true;
}
