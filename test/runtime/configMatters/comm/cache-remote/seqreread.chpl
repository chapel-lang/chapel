use Time;

config const n = 10000;
extern proc chpl_cache_print();
extern proc printf(fmt: c_string, vals...?numvals): int;

proc doit(memory:locale, running:locale) {
  on memory {
    var A:[1..n] int;
    for i in 1..n {
      A[i] = i;
    }
    on running {
      for i in 1..n {
        assert(A[1] == 1);
        assert(A[n-1] == n-1);
        if( A[i] != i ) {
          chpl_cache_print();
          printf("Error reading %i at %p\n", i:c_int, c_ptrTo(A[i]));
          sleep(1);
          chpl_cache_print();
        }
      }
    }
  }
}

doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);
