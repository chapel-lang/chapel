// The purpose of this test is to check that stable sorting
// is in fact stable. It does that by generating random keys
// in a restricted range and keeping around original indices.
// When comparing, if the keys are equal, the indices should be <.

use BlockDist, ChplConfig, Sort, Random;

config type eltType = int;
config const N = 10_000_000;
config const M = 10;
const D = if CHPL_COMM != "none"
          then blockDist.createDomain(0..<N)
          else {0..<N};

var R:[D] eltType;
fillRandom(R, min=0, max=M, seed=1);

record myElt {
  var key: eltType;
  var val: int;
}

var A:[D] myElt;

forall (a, r, idx) in zip(A, R, D) {
  a.key = r;
  a.val = idx;
}

record myKeyComparator : keyComparator {
  proc key(elt: myElt) { return elt.key; }
}
record myRelativeComparator : relativeComparator {
  proc compare(a: myElt, b: myElt) {
    const a0 = a.key;
    const b0 = b.key;
    if a0 < b0 then return -1;
    else if a0 > b0 then return 1;
    else return 0;
  }
}


proc checkSorted(B, stable:bool) {
  forall idx in D {
    if idx > 0 {
      // check that it's sorted by the key
      assert(B[idx-1].key <= B[idx].key);
      if stable && B[idx-1].key == B[idx].key {
        assert(B[idx-1].val < B[idx].val);
      }
    }
  }
}

var B:[D] myElt;

writeln("checking unstable sort with key comparator");
B = A;
sort(B, comparator=new myKeyComparator(), stable=false);
checkSorted(B, stable=false);

writeln("checking unstable sort with relative comparator");
B = A;
sort(B, comparator=new myRelativeComparator(), stable=false);
checkSorted(B, stable=false);

writeln("checking stable sort with key comparator");
B = A;
sort(B, comparator=new myKeyComparator(), stable=true);
checkSorted(B, stable=true);

writeln("checking stable sort with relative comparator");
B = A;
sort(B, comparator=new myRelativeComparator(), stable=true);
checkSorted(B, stable=true);
