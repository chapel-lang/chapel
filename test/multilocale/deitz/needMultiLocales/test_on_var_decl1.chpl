extern proc printf(x...);

proc main {
  on Locales(1) var x = 12;
  printf("%s\n", here.id + " x=" + x);
  on Locales(1) {
    printf("%s\n", here.id + " x=" + x);
  }
}
