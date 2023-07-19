use choiceTestUtils, Random;

proc main() {
  var pcg = createRandomStream(real, algorithm=RNG.PCG, seed=42);
  runTests(pcg);
  var pcgInt = createRandomStream(int, algorithm=RNG.PCG, seed=420);
  runTests(pcgInt);
}

proc runTests(stream) {
  test(stream, [1], trials=10);
  test(stream, [1, 2, 3]);

  var uints: [1..3] uint = [100:uint, 100:uint, 400:uint];
  test(stream, uints);

  var real32s: [1..4] real(32) = [0.1:real(32), 0.2:real(32), 0.3:real(32), 0.4:real(32)];
  test(stream, real32s);

  // user-defined type
  var rArr = [new R(1), new R(2), new R(3)];
  test(stream, rArr, trials=1);

  var pArr = [1, 1, 2];
  test(stream, rArr, prob=pArr, size=2, replace=false, trials=1);

  // offset & strided domain
  var strideDom = {10..#10 by 3};
  var strideArr: [strideDom] int;
  var strideProb: [strideDom] real;
  for i in strideDom {
    strideArr[i] = i;
    strideProb[i] = i;
  }


  test(stream, strideArr, size=20);
  test(stream, strideArr, prob=strideProb, size=20);

  // prob array
  test(stream, [1,2], prob=[0, 1], trials=10);
  test(stream, [1,2], prob=[0.1, 0.9]);
  test(stream, [1,1,2], prob=[0.1, 0.4, 0.5]);

  // Numeric types
  var preal32: [0..1] real(32) = 1;
  test(stream, [1,2], prob=preal32, size=1000, trials=1);
  var pint: [0..1] int = 1;
  test(stream, [1,2], prob=pint, size=1000, trials=1);
  var pint32: [0..1] int(32) = 1;
  test(stream, [1,2], prob=pint32, size=1000, trials=1);
  var puint32: [0..1] uint(32) = 1;
  test(stream, [1,2], prob=puint32, size=1000, trials=1);
  var puint16: [0..1] uint(16) = 1;
  test(stream, [1,2], prob=puint16, size=1000, trials=1);

  // replace=false
  test(stream, [1,2,3,4], size=4, replace=false, trials=1);

  //// domain-type size
  var A = [1,2,3,4];
  var p = [0.1, 0.2, 0.3, 0.4];
  test(stream, A, prob=p, size={0..3});
  test(stream, A, prob=p, size={0..3, 0..2});
  test(stream, A, prob=p, size={0..3, 0..2, 0..1});

  // Ensure we maintain reference to domain passed in
  var dom = {0..4};
  var B: [dom] int;

  var ret = stream.choice([1,2,3,4,5], size=dom);

  dom = {0..9};
  if ret.size != 10 then
    writeln('error: domain reference not maintained');
}

/* User-defined type */
record R {
  var value = 0;
  var tag = "someValue";
}
