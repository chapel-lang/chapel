
extern proc printit(x:c_ptr(c_int), n:c_int);

proc go() {
  var a = for i in 1..10 do i:c_int;

  writeln(a);
  printit(c_ptrTo(a[1]), 10);
  printit(c_ptrTo(a), 10);
}

go();
