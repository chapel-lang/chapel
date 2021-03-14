
proc useit(ARG1, ARG2) return ARG2;

const idx1 = 1101;   // intentionally the same names as ind vars below
const idx2 = 1102;
const idx3 = "1103";

proc main {
  forall (idx1, (idx2, idx3)) in myIter() with (
    var tpv1= idx1,
    var tpv2= idx2,
    var tpv3: idx3.type
  ) {
    writeln((idx1, idx2, idx3, tpv1, tpv2, tpv3));
  }
}

iter myIter() { yield ("hi", (321, 5.5)); }
iter myIter(param tag) { yield ("hello", (1234, 7.7)); }
