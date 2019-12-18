use SysCTypes;

extern proc printarr(x:c_ptr(c_int), n:c_int);

proc go() {
  var t:10*c_int;
  for i in 1..10 do t[i] = i:c_int;

  writeln(t);
  printarr(c_ptrTo(t[1]), 10);
  printarr(c_ptrTo(t):c_ptr(c_int), 10);
}

go();
