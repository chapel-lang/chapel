use Time;

extern proc printf(fmt: c_string, x...);

proc foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", (here.id + " x=" + x).c_str());
    }
  }
  printf("%s\n", (here.id + " x=" + x).c_str());
}

foo();
sleep(4);
