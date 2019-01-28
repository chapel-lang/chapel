use Random;

config const debug = false;

proc main() {
  var pcg = makeRandomStream(real, algorithm=RNG.PCG);
  runTests(pcg);
  var npb = makeRandomStream(real, algorithm=RNG.NPB);
  runTests(npb);
}

proc runTests(stream) {
  testArray(stream, [1], trials=10);
  testArray(stream, [2, 2], trials=10);
  testArray(stream, [0, 1], trials=10);
  testArray(stream, [0.1, 0.9]);
  testArray(stream, [4, 16]);
  testArray(stream, [1, 1, 2]);
  testArray(stream, [2.0001, 2.0002]);
  testArray(stream, [1,2], t=real(64));
  testArray(stream, [1,2], t=real(32));
  testArray(stream, [1,2], t=int(64));
  testArray(stream, [1,2], t=int(32));
  testArray(stream, [1,2], t=uint(32));
  testArray(stream, [1,2], t=uint(16));

  var uints: [1..3] uint = [100:uint, 100:uint, 400:uint];
  testArray(stream, uints);

  var real32s: [1..4] real(32) = [0.1:real(32), 0.2:real(32), 0.3:real(32), 0.4:real(32)];
  testArray(stream, real32s);

  // prob overload
  testArray(stream, [1,2], [0, 1], trials=10);
  testArray(stream, [1,2], [0.1, 0.9]);
  testArray(stream, [1,1,2], [0.1, 0.4, 0.5]);
  testArray(stream, [1,2], [10, 90], t=real(32));
  testArray(stream, [1,2], [10, 90], t=int(32));
  testArray(stream, [1,2], [10, 90], t=uint(32));
  testArray(stream, [1,2], [10, 90], t=uint(16));

  // choices
  testArray(stream, [1,2], [0, 1], trials=10, sampleSize=100);
  testArray(stream, [1,1,2], [0.1, 0.4, 0.5], sampleSize=100);
  testArray(stream, [2, 2], trials=10, sampleSize=100);
  testArray(stream, [0, 1], trials=10, sampleSize=100);
}

proc testArray(stream, arr: [] ?eltType, type t=eltType, trials=10000, sampleSize=0) throws {
  var d = {1..0};
  var prob: [d] real;
  var typedArr: [arr.domain] t = arr: t;
  testArray(stream, typedArr, prob, trials=trials);
}


proc testArray(stream, arr: [], prob: [] ?eltType, type t=eltType, trials=10000, sampleSize=0) throws {
  var typedProb: [prob.domain] t = prob: t;
  var counts = runTrials(stream, arr, typedProb, trials, sampleSize);

  if debug {
    writeln('Counts for array: ', arr);
    for value in counts.domain {
      writeln(value, ' : ', counts[value]/trials:real);
    }
  }

  var actualRatios = if sampleSize == 0 then counts / trials:real
                     else counts / (trials*sampleSize): real;

  // Confirm that resulting ratios are within 0.05 of expected ratios
  var expectedRatios = if prob.size > 0 then getExpectedRatios(arr, prob) else
                                             getExpectedRatios(arr, arr);

  var success = true;
  for value in actualRatios.domain {
    if !isClose(actualRatios[value], expectedRatios[value]) {
      writeln('Choice counts are not within 5% of expected value');
      writeln('value: ', value);;
      writeln('expected %: ', expectedRatios[value]);;
      writeln('actual %: ', actualRatios[value]);;
      success = false;
    }
  }
  if !success {
    writeln('Failed with input:');
    writeln('arr =  ', arr);
    if prob.size > 1 {
      writeln('prob = ', prob);
    }
  }
}

proc getExpectedRatios(arr: [], prob: []) throws {
  var uniqueValues: domain(arr.eltType);
  var expectedRatios: [uniqueValues] real;

  var total = (+ reduce prob):real;

  for i in arr.domain{
    expectedRatios[arr[i]] += prob[i]:real / total;
  }

  return expectedRatios;
}


proc runTrials(stream, arr: [], prob: [],  trials: int, sampleSize: int) throws {
  var countsDom: domain(arr.eltType);
  var counts: [countsDom] int;

  if sampleSize == 0 {
    if prob.size > 0 {
      for 1..trials {
        var c = stream.choice(arr, prob);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var c = stream.choice(arr);
        counts[c] += 1;
      }
    }
  } else {
    if prob.size > 0 {
      for 1..trials {
        var cs = stream.choices(arr, prob, sampleSize);
        for c in cs do counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choices(arr, sampleSize);
        for c in cs do counts[c] += 1;
      }
    }
  }
  return counts;
}

/* This should really be part of the standard library! */
proc isClose(a: real, b: real, epsilon=0.05) {
  return abs(a - b) < epsilon;
}
