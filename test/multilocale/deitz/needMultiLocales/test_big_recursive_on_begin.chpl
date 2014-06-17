config const n: int = 1024;

extern proc printf(x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", here.id + " pre " + i);
    sync {
      on Locales(i%2) do begin foo(i+1);
    }
    printf("%s\n", here.id + " post " + i);
  }
}

foo(1);
