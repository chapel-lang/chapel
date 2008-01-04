use Time;

def main {
  on Locales(1) {
    begin {
      sleep(2);
      on Locales(0) {
        begin {
          sleep(4);
          _debugWriteln("executing on locale ", localeID());
        }
      }
      sleep(2);
      _debugWriteln("executing on locale ", localeID());
    }
  }
  _debugWriteln("executing on locale ", localeID());
}
