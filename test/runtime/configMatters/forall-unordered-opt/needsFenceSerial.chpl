config const N = 1000;
config const useUnorderedAtomics = false;

proc serialTest() {
  var A:[1..N] atomic int;

  on Locales[numLocales-1] {
    serial {
      if useUnorderedAtomics {
        use UnorderedAtomics;
        forall i in 1..N {
          A[i].unorderedAdd(1);
        }
      } else {
        forall i in 1..N {
          A[i].add(1);
        }
      }
    }
  }

  for i in 1..N {
    assert(A[i].read() == 1);
  }
}
serialTest();
