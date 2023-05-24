config const n: int = 1024;

extern proc printf(fmt: c_string, x...);

proc foo(i: int) {
  if i < n {
    printf("%s\n", c_ptrToConst_helper(here.id + " pre " + i):c_string);
    sync {
      on Locales(i%2) do begin foo(i+1);
    }
    printf("%s\n", c_ptrToConst_helper(here.id + " post " + i):c_string);
  }
}

foo(1);
