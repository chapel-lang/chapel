use choiceTestUtils, Random;

proc main() {
  var pcg = createRandomStream(real, algorithm=RNG.PCG, seed=42);
  runTests(pcg);
  var pcgInt = createRandomStream(int, algorithm=RNG.PCG, seed=420);
  runTests(pcgInt);
}

proc runTests(stream) {
  const vec = 1..7 by 2;
  const vec1 = 2..8 by 3;
  const vec2 = 1..3;
  const vec3 = 1..30;
  var D1 = {vec,vec1,vec2};
  var D3 = {vec3};
  var testDom = {vec2,vec2};
  var Prob:[D1] real;
  var sum = 0: real;

  for (i,j,k) in D1 {
    Prob(i,j,k) = (i + j + k + 1): real;
    sum += Prob(i,j,k);
  }

  for (i,j,k) in D1 do
    Prob(i,j,k)/=sum;

  //Multi Dimensional Domain
  test(stream, D1, size=testDom, false, prob=Prob);
  test(stream, D1, size=testDom, true, prob=Prob);

  test(stream, D1, size=2, false, prob=Prob);
  test(stream, D1, size=5, true, prob=Prob);

  test(stream, D1, size=testDom, false);
  test(stream, D1, size=testDom);

  test(stream, D1, size=3);
  test(stream, D1, size=2, false);
  test(stream, D1, size=8, false);

  //Single Dimensional Domain
  test(stream, D3, size=testDom, false);
  test(stream, D3, size=testDom);

  test(stream, D3, size=3);
  test(stream, D3, size=2, false);
  test(stream, D3, size=8, false);

  test(stream, {1..1}, trials=10);
  test(stream, {1..3});


  // offset & strided domain
  var strideDom = {2..10 by 2};
  test(stream, strideDom, size=20);
  test(stream, strideDom, prob=[0, 1, 2, 3, 4], size=20);

  // prob array
  test(stream, {1..2}, prob=[0, 1], trials=10);
  test(stream, {1..2}, prob=[0.1, 0.9]);
  test(stream, {1..3}, prob=[0.1, 0.4, 0.5]);

  // replace=false
  test(stream, {1..4}, size=4, replace=false, trials=1);

  // domain-type size
  var A = {1..4};
  var p = [0.1, 0.2, 0.3, 0.4];
  test(stream, A, prob=p, size={1..4});
  test(stream, A, prob=p, size={1..4, 1..3});
  test(stream, A, prob=p, size={1..4, 1..3, 1..2});

  // Numeric types
  var preal32: [0..1] real(32) = 1;
  test(stream, {1..2}, prob=preal32, size=1000, trials=1);
  var pint: [0..1] int = 1;
  test(stream, {1..2}, prob=pint, size=1000, trials=1);
  var pint32: [0..1] int(32) = 1;
  test(stream, {1..2}, prob=pint32, size=1000, trials=1);
  var puint32: [0..1] uint(32) = 1;
  test(stream, {1..2}, prob=puint32, size=1000, trials=1);
  var puint16: [0..1] uint(16) = 1;
  test(stream, {1..2}, prob=puint16, size=1000, trials=1);
}
