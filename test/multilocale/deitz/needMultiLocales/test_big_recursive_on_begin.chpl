config const n: int = 1024;

extern proc printf(fmt: c_string, x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", (here.id + " pre " + i).c_str());
    sync {
      on Locales(i%2) do begin foo(i+1);
    }
    printf("%s\n", (here.id + " post " + i).c_str());
  }
}

foo(1);
