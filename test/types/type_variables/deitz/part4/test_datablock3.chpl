var d : _ddata(int) = _ddata_allocate(int, 3);

d(0) = 1;
d(1) = 2;
d(2) = 3;

writeln(d(0));
writeln(d(1));
writeln(d(2));

_ddata_free(d, 3);
