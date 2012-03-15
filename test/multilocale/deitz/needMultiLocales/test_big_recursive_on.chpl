config const n: int = 768;

extern proc printf(x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", here.id + " pre " + i);
    on Locales(i%2) do foo(i+1);
    printf("%s\n", here.id + " post " + i);
  }
}

foo(1);
