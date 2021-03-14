extern proc chpl_cache_print();
config const verbose=false;

proc doit(a:locale, b:locale, c:locale)
{
  use SysCTypes;
  extern proc printf(fmt: c_string, vals...?numvals): int;
 
  on a {
    if verbose then printf("on %d\n", here.id:c_int);
    var x = 17;
    var y = 29;
    on b {
      cobegin with (ref x, ref y) {
        { x = 99; }
        { y = 124; }
      }
      assert( x == 99 );
      assert( y == 124 );
    }
    assert( x == 99 );
    assert( y == 124 );
  }
}

doit(Locales[1], Locales[0], Locales[2]);
doit(Locales[1], Locales[2], Locales[0]);
doit(Locales[0], Locales[1], Locales[2]);
doit(Locales[0], Locales[2], Locales[1]);
doit(Locales[2], Locales[0], Locales[1]);
doit(Locales[2], Locales[1], Locales[0]);

