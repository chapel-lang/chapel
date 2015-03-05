use Time;

extern proc printf(fmt: c_string, x...);

proc main {
  foo();
  sleep(5); // make sure program does not exit
}

proc foo() {
  var x: int = 17;
  printf("%s\n", (here.id + " x=" + x).c_str());
  x += 1;
  on Locales(1) {
    begin with (ref x) {
      sleep(2);
      printf("%s\n", (here.id + " x=" + x).c_str());
      x += 1;
      printf("%s\n", (here.id + " x=" + x).c_str());
    }
  }
  printf("%s\n", (here.id + " x=" + x).c_str());
}
