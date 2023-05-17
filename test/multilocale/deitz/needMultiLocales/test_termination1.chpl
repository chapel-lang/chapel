use Time;

extern proc printf(fmt: c_string, x...);

proc main {
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", ("executing on locale " + here.id:string):c_ptrConst(c_char):c_string);
    }
  }
  printf("%s\n", ("executing on locale " + here.id:string):c_ptrConst(c_char):c_string);
}
