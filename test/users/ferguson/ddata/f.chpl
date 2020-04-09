use SysCTypes;

var a:_ddata(c_int);
var size = 10;

a = _ddata_allocate(c_int, size);

if a == nil {
} else {
  writeln("OK");
}

_ddata_free(a, size);
