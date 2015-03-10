extern proc chpl_cache_print();
config const verbose=false;
config const max=100;

proc doit(a:locale, b:locale, c:locale)
{
  extern proc printf(fmt: c_string, vals...?numvals): int;
 
  on a {
    if verbose then printf("on %d\n", here.id:c_int);
    var x:int;
    var stopgo: atomic int;

    // Not totally necessary, but for emphasis.
    x = 0;
    stopgo.write(0);

    sync {
      begin with (ref x) on b {
        for i in 0..max by 2 {
          stopgo.waitFor(i, memory_order_acquire);
          var myx = x;
          if myx != i then printf("ON B GOT %i EXPECTED %i\n", myx:c_int, i:c_int);
          assert( myx == i );
          x = i + 1;
          stopgo.write(i+1, memory_order_release);
        }
      }

      begin with (ref x) on c {
        for i in 1..max by 2 {
          stopgo.waitFor(i, memory_order_acquire);
          var myx = x;
          if myx != i then printf("ON C GOT %i EXPECTED %i\n", myx:c_int, i:c_int);
          assert( myx == i );
          x = i + 1;
          stopgo.write(i+1, memory_order_release);
        }
      }
    }
  }
}

doit(Locales[1], Locales[0], Locales[2]);
doit(Locales[1], Locales[2], Locales[0]);
doit(Locales[0], Locales[1], Locales[2]);
doit(Locales[0], Locales[2], Locales[1]);
doit(Locales[2], Locales[0], Locales[1]);
doit(Locales[2], Locales[1], Locales[0]);

