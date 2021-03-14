config const n: int = 768;

extern proc printf(fmt: c_string, x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", (here.id + " pre " + i).c_str());
    on Locales(i%2) do foo(i+1);
    printf("%s\n", (here.id + " post " + i).c_str());
  }
}

foo(1);
