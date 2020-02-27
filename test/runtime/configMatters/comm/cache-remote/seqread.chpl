use SysCTypes;

config const n = 40000;
extern proc chpl_cache_print();
extern proc printf(fmt: c_string, vals...?numvals): int;
config const verbose=false;

proc doit(memory:locale, running:locale) {
  on memory {
    var A:[1..n] int;
    for i in 1..n {
      A[i] = i;
    }
    on running {
      for i in 1..n {
        var got = A[i];
        if verbose {
          printf("on %d, reading a[%d] got %d\n",
                 here.id:c_int, i:c_int, got:c_int);
          //chpl_cache_print();
        } 
        assert(got == i);
      }
    }
  }
}

//doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);

