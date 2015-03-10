extern proc printf(fmt: c_string, x...);

proc main {
  on Locales(1) var x = 12;
  printf("%s\n", (here.id + " x=" + x).c_str());
  on Locales(1) {
    printf("%s\n", (here.id + " x=" + x).c_str());
  }
}
