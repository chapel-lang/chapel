writeln(__primitive("sizeof_ddata", _ddata(int(32))));

writeln(__primitive("sizeof_ddata", _ddata(int(64))));

writeln(__primitive("sizeof_ddata", _ddata(2*real)));

record rec_t {
  var eye, jay, kay, ell, em, en, oh: int(32);
};
writeln(__primitive("sizeof_ddata", _ddata(rec_t)));

class cls_t {
  var i: 20*int;
  proc i(n) return i(n);
};
writeln(__primitive("sizeof_ddata", _ddata(cls_t)));
