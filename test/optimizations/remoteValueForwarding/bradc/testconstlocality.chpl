use BlockDist;

const GLOBCONST = 2**4;

config const n = 10;
config const checkSync = false;

var sync$: sync bool = true;

const D = {1..n} dmapped Block({1..n});

var A: [D] real;

testit(GLOBCONST);

proc testit(valblc: int) {
  forall (i,a) in zip(D,A) {
    local {
      a = valblc+i/10.0;
      if (here.id == 0) {
        sync$.readFF();
      }
    }
  }
}

writeln("A is: ", A);
