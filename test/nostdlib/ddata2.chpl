proc main() {
  var ddata = _ddata_allocate(int, 5);
  ddata[2] = 7;
  var i = ddata[2];
  var str = i:string;
  __primitive("chpl_error", str);
}

