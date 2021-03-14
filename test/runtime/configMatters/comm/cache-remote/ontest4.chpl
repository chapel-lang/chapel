use SysCTypes;
extern proc chpl_cache_print();
extern proc chpl_cache_fence(acquire:c_int, release:c_int, ln:int(32), fn:int(32));
extern proc printf(fmt: c_string, vals...?numvals): int;
config const verbose=false;

var barriers:[1..16] int;

proc do_barrier(i:int, testnum:int)
{
  var acquire_num = 2*testnum;
  var release_num = acquire_num + 1;
  var acquire = ((i>>acquire_num) & 1):c_int;
  var release = ((i>>release_num) & 1):c_int;
  if verbose && acquire!=0 then printf("on %d running acquire barrier\n", here.id:c_int);
  if verbose && release!=0 then printf("on %d running release barrier\n", here.id:c_int);
  chpl_cache_fence(acquire, release, -1, 0);
}

proc doit(a:locale, b:locale, i:int)
{
  on a {
    if verbose then printf("on %d\n", here.id:c_int);
    var x = 17;
    var y = 100;
    var z = 200;
    on b {

      if verbose then printf("on %d, reading x and y\n", here.id:c_int);
      do_barrier(i, 0);
      var myx = x;
      if verbose then printf("on %d x = %d\n", here.id:c_int, myx:c_int);
      assert(myx == 17);
      var myy = y;
      if verbose then printf("on %d y = %d\n", here.id:c_int, myy:c_int);
      assert(myy == 100);

      if verbose then printf("on %d, setting x = 24 and reading it back\n", here.id:c_int);
      x = 24;
      do_barrier(i, 1);
      if verbose then chpl_cache_print();
      myx = x;
      if verbose then printf("on %d x = %d\n", here.id:c_int, myx:c_int);
      assert(myx == 24);

      if verbose then printf("on %d, setting x = 25 and reading it back\n", here.id:c_int);
      x = 25;
      do_barrier(i, 2);
      if verbose then chpl_cache_print();
      myx = x;
      if verbose then printf("on %d x = %d\n", here.id:c_int, myx:c_int);
      assert(myx == 25);

      if verbose then printf("on %d, setting y = 99 and reading it back\n", here.id:c_int);
      y = 99;
      do_barrier(i, 3);
      if verbose then chpl_cache_print();
      myy = y;
      if verbose then printf("on %d y = %d\n", here.id:c_int, myy:c_int);
      assert(myy == 99);
      do_barrier(i, 3);

      if verbose then printf("on %d, setting z = 201 and reading it back\n", here.id:c_int);
      z = 201;
      do_barrier(i, 4);
      if verbose then chpl_cache_print();
      var myz = z;
      if verbose then printf("on %d z = %d\n", here.id:c_int, myz:c_int);
      assert(myz == 201);
      do_barrier(i, 3);

      if verbose then printf("on %d, setting x = 26 and reading it back\n", here.id:c_int);
      x = 26;
      do_barrier(i, 5);
      if verbose then chpl_cache_print();
      myx = x;
      if verbose then printf("on %d x = %d\n", here.id:c_int, myx:c_int);
      assert(myx == 26);
    }
    assert(x == 26);
    assert(y == 99);
    assert(z == 201);
  }
}

var n = (1<<10);
for i in 0..#n {
  if verbose then printf("Barriers is 0x%x\n", i:c_int);
  doit(Locales[1], Locales[0], i);
}

