config const n = 10000;
config const verbose = false;
extern proc chpl_cache_print();

proc doit(memory:locale, running:locale) {
  on memory {
    var A:[1..n] int;

    for i in 1..n {
      A[i] = i;
    }
 
    on running {
      for i in 1..n {
        A[i] = i;
        var j = 1 + ((2*i+7) % n);
        var aj = A[j];
        if aj != j {
          if verbose then chpl_cache_print();
          writeln("error A[",j,"]=",aj);
          assert(false);
        }
        assert(A[j] == j);
      }
    }
    for i in 1..n {
      assert(A[i] == i);
    }
  }
}

doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);
