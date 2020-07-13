use common;

proc F(i) {
  return i*2;
}

class MyClass {
  proc this(i) {
    return i;
  }
}

{

  const space = {1..10};
  var dom = createDom(space);

  var A: [dom] real;
  var B: [dom] real;
  var C = createArr(space, real);
  var D: [space] real;
  var E = new MyClass();


  B = 3;

  // this is the very basic case and optimzed completely
  forall (i, loopIdx) in zip(dom, 1..) {
    A[i] = B[i] +
           C[i] +
           D[i] +
           E[i] +
           F[i]*loopIdx;  // functions are unresolved sym exprs, so they are skipped
  }
  writeln(A);
}
