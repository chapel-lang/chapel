extern record rec_t {
  var mem: int(64);
};

export
proc i2rec(i: int) {
  var r: rec_t;
  r.mem = i;
  return r;
}

writeln("i2rec(2).mem: ", i2rec(2).mem);
