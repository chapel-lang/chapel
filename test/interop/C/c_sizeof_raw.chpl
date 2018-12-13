var i32: int(32);
writeln(c_sizeof_raw(i32));

var i64: int(64);
writeln(c_sizeof_raw(i64));

var tup: 2*real;
writeln(c_sizeof_raw(tup));

record rec_t {
  var eye, jay, kay, ell, em, en, oh: int(32);
};
writeln(c_sizeof_raw(rec_t));

class cls_t {
  var i: 20*int;
  proc i(n) return i(n);
};
writeln(c_sizeof_raw(cls_t));
