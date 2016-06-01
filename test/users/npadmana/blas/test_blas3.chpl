use Random;
use BLAS;
use C_BLAS;

config const errorThresholdDouble = 1.e-10;
config const errorThresholdSingle = 1.e-5;

proc main() {
  test_gemm();
}

proc test_gemm() {
  test_gemm_helper(real(32));
  test_gemm_helper(real(64));
  test_gemm_helper(complex(64));
  test_gemm_helper(complex(128));
}

proc test_gemm_helper(type t) {
  var passed = 0, 
      failed = 0,
      tests = 0;
  const errorThreshold = blasError(t);

  {
    const m = 10 : c_int,
          n = 7 : c_int,
          k = 7 : c_int;
    // Test dgemm -- do this with an array that isn't square
    var A : [{0.. #m, 0.. #k}]t,
        B : [{0.. #k, 0.. #n}]t,
        C : [{0.. #m, 0.. #n}]t,
        D : [{0.. #m, 0.. #n}]t;

    var rng = makeRandomStream(eltType=t,algorithm=RNG.PCG);
    rng.fillRandom(A);
    rng.fillRandom(B);
    rng.fillRandom(C);
    D = C;
    const alpha = rng.getNext(),
          beta = rng.getNext();

    gemm(A,B,C,alpha,beta);
    forall (i,j) in D.domain do D[i,j] = beta*D[i,j]+alpha*(+ reduce (A[i,..]*B[..,j]));
    var err = max reduce abs(C-D);
    if err > errorThreshold {
      failed += 1;
      writef("%sgemm : Failure on test %i : %r\n",blasPrefix(t), tests, err);
    } else {
      passed += 1; 
    }
  }

  writef("%sgemm : %i PASSED, %i FAILED \n", blasPrefix(t), passed, failed);
}

proc blasError(type t) {
  select t {
    when real(32) do return errorThresholdSingle;
    when real(64) do return errorThresholdDouble;
    when complex(64) do return errorThresholdSingle;
    when complex(128) do return errorThresholdDouble;
  }
}

proc blasPrefix(type t) {
  select t {
    when real(32) do return "s";
    when real(64) do return "d";
    when complex(64) do return "c";
    when complex(128) do return "z";
  }
}
