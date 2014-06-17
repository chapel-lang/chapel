proc main() {
  var ddata = _ddata_allocate(int, 5);
  _ddata_free(ddata);
  __primitive("chpl_error", "OK");
}

