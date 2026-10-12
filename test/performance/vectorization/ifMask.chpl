config const N = 50_000_000;
config const iterations = 100;
config const warmup = 50;
config const printTime = true;

use Time;
use Random;
use CTypes;

extern {
  void ifmask_c(long int N, const float* A, const float* B, float* C);
  void __attribute__((noinline))
  ifmask_c(long int N, const float* A, const float* B, float* C) {
    for (long int i = 0; i < N; i++) {
      C[i] = (A[i] >= 0.0f) ? B[i] : 0.0f;
    }
  }
}

record ifmask_c_wrapper {
  inline proc this(A, B, ref C) {
    ifmask_c(A.size, c_ptrToConst(A), c_ptrToConst(B), c_ptrTo(C));
  }
}


pragma "no inline"
proc ifmaskOverDomain(A: [?D] ?eltTy, B, ref C) {
  foreach i in D {
    C[i] = if A[i] >= 0:eltTy then B[i] else 0:eltTy;
  }
}
record ifmaskOverDomainWrapper {
  inline proc this(A, B, ref C) {
    ifmaskOverDomain(A, B, C);
  }
}

pragma "no inline"
proc ifmaskOverDomainWithTemps(A: [?D] ?eltTy, B, ref C) {
  foreach i in D {
    const a = A[i];
    const b = B[i];
    C[i] = if a >= 0:eltTy then b else 0:eltTy;
  }
}
record ifmaskOverDomainWithTempsWrapper {
  inline proc this(A, B, ref C) {
    ifmaskOverDomainWithTemps(A, B, C);
  }
}

pragma "no inline"
proc ifmaskOverDomainWithCPtr(A: [?D] ?eltTy, B, ref C) {
  var Cp = c_ptrTo(C);
  var Ap = c_ptrToConst(A);
  var Bp = c_ptrToConst(B);
  foreach i in 0..<D.size {
    const a = Ap[i];
    const b = Bp[i];
    Cp[i] = if a >= 0:eltTy then b else 0:eltTy;
  }
}
record ifmaskOverDomainWithCPtrWrapper {
  inline proc this(A, B, ref C) {
    ifmaskOverDomainWithCPtr(A, B, C);
  }
}

pragma "no inline"
proc ifmaskZippered(A: [] ?eltTy, B, ref C) {
  foreach (a,b,c) in zip(A,B,C) {
    c = if a >= 0:eltTy then b else 0:eltTy;
  }
}
record ifmaskZipperedWrapper {
  inline proc this(A, B, ref C) {
    ifmaskZippered(A, B, C);
  }
}



extern {
  void escape(void* p);
  void escape(void* p) {
    asm volatile("" : : "g"(p) : "memory");
  }
}

pragma "no inline"
proc test(param name, kernelFunctor) {
  var A: [1..N] real(32);
  var B: [1..N] real(32);
  var C: [1..N] real(32);

  fillRandom(A);
  fillRandom(B);

  for 1..#warmup {
    kernelFunctor(A, B, C);
    escape(c_ptrTo(C));
  }

  var t = new stopwatch();
  t.start();

  for 1..#iterations {
    kernelFunctor(A, B, C);
    escape(c_ptrTo(C));
  }
  t.stop();

  if printTime then
    writeln("Time elapsed (", name, "): ", t.elapsed());
}

proc main() {

  test("domain iteration", new ifmaskOverDomainWrapper());
  test("domain iteration with temps", new ifmaskOverDomainWithTempsWrapper());
  test("domain iteration with c_ptr", new ifmaskOverDomainWithCPtrWrapper());
  test("zippered iteration", new ifmaskZipperedWrapper());
  test("external c", new ifmask_c_wrapper());
}

