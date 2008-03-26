use Time;

def foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      sleep(2);
      _debugWriteln(here.id, " x=", x);
    }
  }
  _debugWriteln(here.id, " x=", x);
}

foo();
sleep(4);
