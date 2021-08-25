use CPtr;

// This code is dereved from issue github issue #15817.
// At the time it was written it was failing to compile
// with the LLVM backend with an error like:
//
// DestTy too big for FPTrunc
//   %97 = fptrunc float %96 to double
// internal error: LLVM function verification failed

record R {
  type T;
  var beta: T;
  proc kernel(x: c_ptr) {
    var dist: T = 0:T;
    dist += x[0]**this.beta;
    return 0.5:T;
  }
}

proc main() {
  type t = real(32);
  var A: [0..5] t;
  var r = new R(t);

  var tmp = r.kernel(c_ptrTo(A));
}
