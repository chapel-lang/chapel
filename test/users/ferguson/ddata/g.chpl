var a:_ddata(c_int);
var size = 10;

a = _ddata_allocate(c_int, size);

if a != nil {
  writeln("OK");
} else {
}

_ddata_free(a);
