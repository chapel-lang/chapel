
proc main {
  {
    var c_sublocid_local: int;
    for idx in 1..88 do c_sublocid_local = idx;
    USEMEa(c_sublocid_local);
  }
  {
    // While having the extern be named _chpl
    // is not legitimate, we use it here to enable
    // a consistent .bad file for testing.
    extern var c_sublocid_local, c_sublocid_local_chpl: int;
    USEMEb(c_sublocid_local); USEMEb(c_sublocid_local_chpl);
  }
}

proc USEMEa(ARGa: int) {
  writef("USEMEa %i\n", ARGa);
}
proc USEMEb(ARGb: int) {
  writef("USEMEb %i\n", ARGb);
}
