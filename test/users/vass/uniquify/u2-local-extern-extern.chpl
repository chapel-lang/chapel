
proc main {
  {
    extern var sublocid_local: int;
    USEMEa(sublocid_local);
  }
  {
    extern var sublocid_local: real;
    USEMEb(sublocid_local);
  }
}

proc USEMEa(ARGa: int) {
  writef("USEMEa %i\n", ARGa);
}

proc USEMEb(ARGb: real) {
  writef("USEMEb %r\n", ARGb);
}
