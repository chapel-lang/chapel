use CTypes;

config param N = 10;

on here.gpus[0] {
  const D = {1..N};
  var A: [D] int;
  var B: [D] c_ptr(int);
  var C: [D] c_ptrConst(int);

  foreach i in A.domain {
    A[i] = i;
    B[i] = c_ptrTo(A[i]);
    C[i] = c_ptrToConst(A[i]);
  }

  for i in D {
    writeln(C[i].deref());
  }
}
