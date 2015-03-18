extern proc chpl_cache_print();
config const verbose=false;
config const max=100;

proc doit(a:locale, b:locale, c:locale)
{
  extern proc printf(fmt: c_string, vals...?numvals): int;
 
  on a {
    if verbose then printf("on %d\n", here.id:c_int);
    var x:int;
    var stopgob: sync bool;
    var stopgoc: sync bool;

    // Not totally necessary, but for emphasis.
    x = 0;
    stopgob = true;

    sync {
      begin with (ref x) on b {
        for i in 0..max by 2 {
          var z = stopgob; // wait for it
          assert( x == i );
          x = i + 1;
          stopgoc = true; // signal
        }
      }

      begin with (ref x) on c {
        for i in 1..max by 2 {
          var z = stopgoc; // wait for it
          assert( x == i );
          x = i + 1;
          stopgob = true; // signal
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

