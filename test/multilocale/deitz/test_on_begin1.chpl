use Time;

def foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      _debugWriteln(localeID(), " x=", x);
    }
  }
  sleep(2);
  _debugWriteln(localeID(), " x=", x);
}

foo();
