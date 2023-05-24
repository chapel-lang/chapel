use Time, CTypes;

extern proc printf(fmt: c_string, x...);

proc main {
  foo();
}

proc foo() {
  var x: int = 17;
  printf("%s\n", c_ptrToConst_helper(here.id:string + " x=" + x:string):c_string);
  x += 1;
  // Now x is 18

  on Locales(1) {
    begin with (ref x) {
      // wait for statement A below
      sleep(2);
      // Now x should be 18
      printf("%s\n", c_ptrToConst_helper(here.id:string + " x=" + x:string):c_string);
      x += 1;
      // Now x should be 19
      printf("%s\n", c_ptrToConst_helper(here.id:string + " x=" + x:string):c_string);
    }
  }

  // statement A. x should still be 18 because of the sleep above.
  printf("%s\n", c_ptrToConst_helper(here.id:string + " x=" + x:string):c_string);

  // make sure this stack frame does not exit since x is stack allocated
  sleep(5);
}
