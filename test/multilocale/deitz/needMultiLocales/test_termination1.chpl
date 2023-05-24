use Time, CTypes;

extern proc printf(fmt: c_string, x...);

proc main {
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", c_ptrToConst_helper("executing on locale " + here.id:string):c_string);
    }
  }
  printf("%s\n", c_ptrToConst_helper("executing on locale " + here.id:string):c_string);
}
