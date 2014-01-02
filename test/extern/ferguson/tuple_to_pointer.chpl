
extern proc printit(x:c_ptr(c_int), n:c_int);

proc go() {
  var t:10*c_int;
  for i in 1..10 do t[i] = i:c_int;

  writeln(t);
  printit(c_ptrTo(t[1]), 10);
  printit(c_ptrTo(t), 10);
}

go();
