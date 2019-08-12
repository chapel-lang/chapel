use Time;

extern proc printf(fmt: c_string, x...);

proc foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", (here.id:string + " x=" + x:string).c_str());
    }
  }
  printf("%s\n", (here.id:string + " x=" + x:string).c_str());
}

foo();
sleep(4);
