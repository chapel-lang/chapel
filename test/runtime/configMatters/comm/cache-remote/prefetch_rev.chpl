extern proc chpl_cache_print();
extern proc printf(fmt: c_string, vals...?numvals): int;
config const verbose=false;

on Locales[1] {
  var a:[0..198] int;

  for i in 0..198 {
    a[i] = i;
  }

  on Locales[0] {

    for i in 0..198 by -1 {
      if verbose then printf("on %d, reading a[%d]\n", here.id:c_int, i:c_int);
      var mine = a[i];
      assert(mine == i);
      if verbose then chpl_cache_print();
    }
  }
}


