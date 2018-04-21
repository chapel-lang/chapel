
var c_sublocid_vass: chpl_sublocID_t = -7;

proc main {

  forall IDX in MYITER()
    with (const in c_sublocid_any,
          const in c_sublocid_vass)
  {
    USEME2(c_sublocid_any, c_sublocid_vass);
  }

}

iter MYITER() { yield 5555; }
iter MYITER(param tag) {
  coforall CIDX in DRIVER() {
    yield 6666;
  }
}

iter DRIVER() { yield 7777; }

proc USEME2(ARG1: chpl_sublocID_t, ARG2: chpl_sublocID_t) {
  writef("USEME2 %i %i\n", ARG1, ARG2);
}
