use SysCTypes;

var a:_ddata(c_int);
var size = 10;

a = _ddata_allocate(c_int, size);

for i in 0..#size {
  a[i] = i:c_int;
}

for i in 0..#size {
  writeln(a[i]);
}


_ddata_free(a, size);

writeln("OK");
