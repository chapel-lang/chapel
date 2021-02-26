use SysCTypes, SysBasic;

extern proc printarr(x: [], n:c_int);

proc go() {
  var a = for i in 1..10 do i:c_float;

  writeln(a);
  printarr(a, 10);
}

go();
