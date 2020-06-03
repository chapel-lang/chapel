var ddi32: _ddata(int(32));
writeln(_ddata_sizeof_element(ddi32));

var ddi64: _ddata(int(64));
writeln(_ddata_sizeof_element(ddi64));

writeln(_ddata_sizeof_element(_ddata(2*real)));

record rec_t {
  var eye, jay, kay, ell, em, en, oh: int(32);
};
writeln(_ddata_sizeof_element(_ddata(rec_t)));

class cls_t {
  var i: 20*int;
  proc i(n) return i(n);
};
writeln(_ddata_sizeof_element(_ddata(unmanaged cls_t)));
