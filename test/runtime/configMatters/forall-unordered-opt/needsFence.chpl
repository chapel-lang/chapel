config const N=10000;
config const M=100000;

proc histo_fence() {
  var A: [0..#M] int;
  var B: [0..#M] atomic int;
  var rindex: [0..#N] int;

  cobegin {
    {
      on Locales[numLocales-1] {
        forall r in rindex {
          A[r] = 1;
          B[r].add(1);
        }
      }
    }

    {
      // this task assumes B[r] > 1 implies A[r] = 1
      forall r in rindex {
        if B[r].read() != 0 then
          assert(A[r] == 1);
      }
    }
  }
}
histo_fence();
