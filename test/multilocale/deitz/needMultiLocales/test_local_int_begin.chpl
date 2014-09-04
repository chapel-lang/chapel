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
  on Locales(1) {
    begin with (ref x) {
      sleep(2);
      printf("%s\n", here.id + " x=" + x);
      x += 1;
      printf("%s\n", here.id + " x=" + x);
    }
  }
  printf("%s\n", here.id + " x=" + x);
}
