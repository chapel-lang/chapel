use Random;

config const debug = false;

proc main() {
  testArray([1], trials=10);
  testArray([2, 2], trials=10);
  testArray([0, 1], trials=10);
  testArray([0.1, 0.9]);
  testArray([4, 16]);
  testArray([1, 1, 2]);
  testArray([2.0001, 2.0002]);
  testArray([1,2], t=real(64));
  testArray([1,2], t=real(32));
  testArray([1,2], t=int(64));
  testArray([1,2], t=int(32));
  testArray([1,2], t=uint(32));
  testArray([1,2], t=uint(16));

  var uints: [1..3] uint = [100:uint, 100:uint, 400:uint];
  testArray(uints);

  var real32s: [1..4] real(32) = [0.1:real(32), 0.2:real(32), 0.3:real(32), 0.4:real(32)];
  testArray(real32s);

  // prob overload
  testArray([1,2], [0, 1], trials=10);
  testArray([1,2], [0.1, 0.9]);
  testArray([1,1,2], [0.1, 0.4, 0.5]);
  testArray([1,2], [10, 90], t=real(32));
  testArray([1,2], [10, 90], t=int(32));
  testArray([1,2], [10, 90], t=uint(32));
  testArray([1,2], [10, 90], t=uint(16));
}

proc testArray(arr: [] ?eltType, type t=eltType, trials=10000) throws {
  var d = {1..0};
  var prob: [d] real;
  var typedArr: [arr.domain] t = arr: t;
  testArray(typedArr, prob, trials=trials);
}


proc testArray(arr: [], prob: [] ?eltType, type t=eltType, trials=10000) throws {
  var typedProb: [prob.domain] t = prob: t;
  var counts = runTrials(arr, typedProb, trials);

  if debug {
    writeln('Counts for array: ', arr);
    for value in counts.domain {
      writeln(value, ' : ', counts[value]/trials:real);
    }
  }

  var actualRatios = counts / trials:real;

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


proc runTrials(arr: [], prob: [],  trials: int) throws {
  var countsDom: domain(arr.eltType);
  var counts: [countsDom] int;

  if prob.size > 0 {
    for 1..trials {
      var c = choice(arr, prob);
      counts[c] += 1;
    }
  } else {
    for 1..trials {
      var c = choice(arr);
      counts[c] += 1;
    }
  }

  return counts;
}

proc isClose(a: real, b: real, epsilon=0.05) {
  return abs(a - b) < epsilon;
}
