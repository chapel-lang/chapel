use choiceTestUtils, Random;

proc main() {
  var pcg = createRandomStream(real, algorithm=RNG.PCG, seed=42);
  runTests(pcg);
  var pcgInt = createRandomStream(int, algorithm=RNG.PCG, seed=420);
  runTests(pcgInt);
}

proc runTests(stream) {
  test(stream, 1..1, trials=10);
  test(stream, 1..3);


  // offset & strided range
  var strideRng = 2..10 by 2;
  test(stream, strideRng, size=20);
  test(stream, strideRng, prob=[0, 1, 2, 3, 4], size=20);

  // prob array
  test(stream, 1..2, prob=[0, 1], trials=10);
  test(stream, 1..2, prob=[0.1, 0.9]);
  test(stream, 1..3, prob=[0.1, 0.4, 0.5]);

  // replace=false
  test(stream, 1..4, size=4, replace=false, trials=1);

  // domain-type size
  var A = 1..4;
  var p = [0.1, 0.2, 0.3, 0.4];
  test(stream, A, prob=p, size={1..4});
  test(stream, A, prob=p, size={1..4, 1..3});
  test(stream, A, prob=p, size={1..4, 1..3, 1..2});
  
  // Numeric types
  var preal32: [0..1] real(32) = 1;
  test(stream, 1..2, prob=preal32, size=1000, trials=1);
  var pint: [0..1] int = 1;
  test(stream, 1..2, prob=pint, size=1000, trials=1);
  var pint32: [0..1] int(32) = 1;
  test(stream, 1..2, prob=pint32, size=1000, trials=1);
  var puint32: [0..1] uint(32) = 1;
  test(stream, 1..2, prob=puint32, size=1000, trials=1);
  var puint16: [0..1] uint(16) = 1;
  test(stream, 1..2, prob=puint16, size=1000, trials=1);
}
