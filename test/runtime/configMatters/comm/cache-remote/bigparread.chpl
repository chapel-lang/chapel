config const n = 10000;
extern proc chpl_cache_print();

proc doit(memory:locale, running:locale) {
  on memory {
    var A:[1..n] int;
    for i in 1..n {
      A[i] = i;
    }
    on running {
      forall i in 1..n {
        assert(A[i] == i);
      }
    }
  }
}

doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);
