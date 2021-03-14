extern proc chpl_cache_print();
config const verbose=false;

proc doit(a:locale, b:locale)
{
  use SysCTypes;
  extern proc printf(fmt: c_string, vals...?numvals): int;
 
  on a {
    if verbose {
      printf("on %d\n", here.id:c_int);
      chpl_cache_print();
    }
    var x = 17;
    on b {
      var myx = 0;
      x = 24;
      myx = x;
      if verbose {
        printf("on %d x = %d\n", here.id:c_int, myx:c_int);
        chpl_cache_print();
      }
      assert(myx == 24);
    }
    assert(x == 24);
  }
}

doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);
doit(Locales[1], Locales[0]);
doit(Locales[0], Locales[1]);

