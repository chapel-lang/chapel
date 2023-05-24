use Time, CTypes;

extern proc printf(fmt: c_string, x...);

proc foo() {
  var x: int = 2;
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", c_ptrToConst_helper(here.id:string + " x=" + x:string):c_string);
    }
  }
  printf("%s\n", c_ptrToConst_helper(here.id:string + " x=" + x:string):c_string);
}

foo();
sleep(4);
