use SysCTypes;

var a:_ddata(c_int);
var size = 10;

a = _ddata_allocate(c_int, size);

var b = a;

if b == a then writeln("OK");

_ddata_free(a, size);
