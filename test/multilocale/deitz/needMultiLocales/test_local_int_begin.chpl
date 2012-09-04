use Time;

extern proc printf(x...);

proc main {
  foo();
  sleep(5); // make sure program does not exit
}

proc foo() {
  var x: int = 17;
  printf("%s\n", here.id + " x=" + x);
  x += 1;
  on rootLocale.getLocale(1) {
    begin {
      sleep(2);
      printf("%s\n", here.id + " x=" + x);
      x += 1;
      printf("%s\n", here.id + " x=" + x);
    }
  }
  printf("%s\n", here.id + " x=" + x);
}
