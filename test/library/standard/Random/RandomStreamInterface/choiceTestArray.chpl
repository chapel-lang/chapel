use Random, Map;

config const debug = false;

proc main() {
  var pcg = createRandomStream(real, algorithm=RNG.PCG, seed=42);
  runTests(pcg);
  var pcgInt = createRandomStream(int, algorithm=RNG.PCG, seed=420);
  runTests(pcgInt);
}

proc runTests(stream) {
  testArray(stream, [1], trials=10);
  testArray(stream, [1, 2, 3]);

  var uints: [1..3] uint = [100:uint, 100:uint, 400:uint];
  testArray(stream, uints);

  var real32s: [1..4] real(32) = [0.1:real(32), 0.2:real(32), 0.3:real(32), 0.4:real(32)];
  testArray(stream, real32s);

  // user-defined type
  var rArr = [new R(1), new R(2), new R(3)];
  testArray(stream, rArr, trials=1);

  var pArr = [1, 1, 2];
  testArray(stream, rArr, prob=pArr, size=2, replace=false, trials=1);

  // offset & strided domain
  var strideDom = {10..#10 by 3};
  var strideArr: [strideDom] int;
  var strideProb: [strideDom] real;
  for i in strideDom {
    strideArr[i] = i;
    strideProb[i] = i;
  }


  testArray(stream, strideArr, size=20);
  testArray(stream, strideArr, prob=strideProb, size=20);

  // prob array
  testArray(stream, [1,2], prob=[0, 1], trials=10);
  testArray(stream, [1,2], prob=[0.1, 0.9]);
  testArray(stream, [1,1,2], prob=[0.1, 0.4, 0.5]);

  // Numeric types
  var preal32: [0..1] real(32) = 1;
  testArray(stream, [1,2], prob=preal32, size=1000, trials=1);
  var pint: [0..1] int = 1;
  testArray(stream, [1,2], prob=pint, size=1000, trials=1);
  var pint32: [0..1] int(32) = 1;
  testArray(stream, [1,2], prob=pint32, size=1000, trials=1);
  var puint32: [0..1] uint(32) = 1;
  testArray(stream, [1,2], prob=puint32, size=1000, trials=1);
  var puint16: [0..1] uint(16) = 1;
  testArray(stream, [1,2], prob=puint16, size=1000, trials=1);

  // replace=false
  testArray(stream, [1,2,3,4], size=4, replace=false, trials=1);

  //// domain-type size
  var A = [1,2,3,4];
  var p = [0.1, 0.2, 0.3, 0.4];
  testArray(stream, A, prob=p, size={0..3});
  testArray(stream, A, prob=p, size={0..3, 0..2});
  testArray(stream, A, prob=p, size={0..3, 0..2, 0..1});

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

proc testArray(stream, arr: [] ?eltType, size:?sizeType=none, replace=true, prob:?probType=none, trials=10000) throws {
  var counts = new map(eltType, int);

  // Collect statistics
  if isNothingType(probType) {
    if isNothingType(sizeType) {
      for 1..trials {
        var c = stream.choice(arr, replace=replace);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choice(arr, size=size, replace=replace);
        for c in cs do counts[c] += 1;
      }
    }
  } else {
    if isNothingType(sizeType) {
      for 1..trials {
        var c = stream.choice(arr, prob=prob, replace=replace);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choice(arr, prob=prob, size=size, replace=replace);
        for c in cs do counts[c] += 1;
      }
    }
  }


  if debug {
    writeln('Counts for array: ', arr);
    for value in counts {
      writeln(value, ' : ', counts[value]/trials:real);
    }
  }

  var m = 1;
  if isDomainType(sizeType) then m = size.size;
  else if isIntegralType(sizeType) then m = size;

  var actualRatios = new map(eltType, real);
  for (k,v) in actualRatios.items() {
    if isNothingType(sizeType) then
      actualRatios[k] = v / trials:real;
    else
      actualRatios[k] = v / (trials*m): real;
  }

  var ones: [arr.domain] real = 1;

  var probabilities = if isNothingType(prob.type) then ones else prob;

  // Get expected ratios
  var uniqueValues: domain(arr.eltType);
  var expectedRatios = new map(arr.eltType, real);

  var total = (+ reduce probabilities):real;

  for i in arr.domain{
    expectedRatios[arr[i]] += probabilities[i]:real / total;
  }

  // Confirm that resulting ratios are within 0.05 of expected ratios
  var success = true;
  if replace {
    for value in actualRatios {
      if !isClose(actualRatios[value], expectedRatios[value]) {
        success = false;
      }
    }
    if !success {
      writeln('Failed with args:');
      write('choice(');
      write('[', arr, '], ');
      if !isNothingType(prob.type) then write('prob = ', prob, ', ');
      if !isNothingType(size.type) then writeln('size = ', size, ', ');
      writeln('replace = ', replace, ');');
      for value in actualRatios {
        writeln('value   expected   actual');
        writeln(value, '       ', expectedRatios[value:arr.eltType],'        ', actualRatios[value]);
      }
    }
  }
}


/* This should really be part of the standard library! */
proc isClose(a: real, b: real, epsilon=0.05) {
  return abs(a - b) < epsilon;
}
