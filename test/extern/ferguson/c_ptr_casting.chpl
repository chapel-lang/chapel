
use CTypes;

extern proc printarr(x:c_ptr(c_int), n:c_int);

record R {
  var x: c_int;
}

proc go() {
  var a:[1..10] R;

  for i in 1..10 do a[i] = new R(i:c_int);

  writeln(a);
  printarr(c_ptrTo(a[1]):c_void_ptr:c_ptr(c_int), 10);
}

go();

