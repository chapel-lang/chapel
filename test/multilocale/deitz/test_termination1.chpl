use Time;

def main {
  on Locales(1) {
    begin {
      sleep(2);
      _debugWriteln("executing on locale ", localeID());
    }
  }
  _debugWriteln("executing on locale ", localeID());
}
