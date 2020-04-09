record r {
  var x:int;
  var y:int;
};

var a:_ddata(r);
var size = 10;

a = _ddata_allocate(r, size);

if a then writeln("OK");

_ddata_free(a, size);
