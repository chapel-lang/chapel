use SysCTypes;

extern proc sumarr(x: [] c_int, n:c_int): int;
extern proc printarr(x: [] c_int, n:c_int);

proc go() {
  var a = for i in 1..10 do i:c_int;

  writeln(a);
  printarr(a, 10);
  var x = sumarr(a, 10);
  writeln(x);
}

go();
