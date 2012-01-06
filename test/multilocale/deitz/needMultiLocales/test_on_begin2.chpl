use Time;

extern proc printf(x...);

proc foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", here.id + " x=" + x);
    }
  }
  printf("%s\n", here.id + " x=" + x);
}

foo();
sleep(4);
