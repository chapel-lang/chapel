use SysCTypes;

config const n = 40000;
extern proc chpl_cache_print();
extern proc printf(fmt: c_string, vals...?numvals): int;
config const verbose=false;

proc doit(memory:locale, running:locale) {
  on memory {
    var A:[1..n] int;
    var B:[1..n] int;
    for i in 1..n {
      A[i] = i;
      B[i] = 2*i;
    }
    on running {
      for i in 1..n {
        var gota = A[i];
        var gotb = B[i];
        if verbose {
          printf("on %d, reading a[%d] got %d\n",
                 here.id:c_int, i:c_int, gota:c_int);
          //chpl_cache_print();
        } 
        assert(gota == i);
        assert(gotb == 2*i);
      }
    }
  }
}

//doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);

