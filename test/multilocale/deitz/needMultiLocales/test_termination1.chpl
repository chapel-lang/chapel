use Time;

extern proc printf(x...);

proc main {
  on Locales(1) {
    begin {
      sleep(2);
      printf("%s\n", "executing on locale " + here.id);
    }
  }
  printf("%s\n", "executing on locale " + here.id);
}
