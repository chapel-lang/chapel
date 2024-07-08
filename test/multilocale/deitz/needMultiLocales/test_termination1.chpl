use Time, CTypes;

extern proc printf(fmt: c_ptrConst(c_char), x...);

proc main {
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", ("executing on locale " + here.id:string).c_str());
    }
  }
  printf("%s\n", ("executing on locale " + here.id:string).c_str());
}
