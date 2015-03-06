extern proc chpl_cache_print();
config const verbose=false;

proc doit(a:locale, b:locale, c:locale)
{
  extern proc printf(fmt: c_string, vals...?numvals): int;
 
  on a {
    if verbose then printf("on %d\n", here.id:c_int);
    var A = [1,2,3,4,5,6,7,8,9,10];
    on b {
      for i in 1..10 {
        A[i] = 3*i;
      }

      forall i in 1..10 {
        on c {
          assert( A[i] == 3*i );
          A[i] = 2*i;
          assert( A[i] == 2*i );
        }
      }

      for i in 1..10 {
        assert(A[i] == 2*i);
      }
    }

    for i in 1..10 {
      assert(A[i] == 2*i);
    }
  }
}

doit(Locales[1], Locales[0], Locales[2]);
doit(Locales[1], Locales[2], Locales[0]);
doit(Locales[0], Locales[1], Locales[2]);
doit(Locales[0], Locales[2], Locales[1]);
doit(Locales[2], Locales[0], Locales[1]);
doit(Locales[2], Locales[1], Locales[0]);

