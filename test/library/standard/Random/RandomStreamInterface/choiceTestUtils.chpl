use Random, Map;

config const debug = false;

proc test(stream, X, size:?sizeType=none, replace=true, prob:?probType=none, trials=10000) throws {
  var counts;
  if isArrayValue(X) {
    counts = new map(X.eltType, int);
  } else {
    counts = new map(int, int);
  }

  // Collect statistics
  if isNothingType(probType) {
    if isNothingType(sizeType) {
      for 1..trials {
        var c = stream.choice(X, replace=replace);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choice(X, size=size, replace=replace);
        for c in cs do counts[c] += 1;
      }
    }
  } else {
    if isNothingType(sizeType) {
      for 1..trials {
        var c = stream.choice(X, prob=prob, replace=replace);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choice(X, prob=prob, size=size, replace=replace);
        for c in cs do counts[c] += 1;
      }
    }
  }


  if debug {
    writeln('Counts for X: ', X);
    for value in counts {
      writeln(value, ' : ', counts[value]/trials:real);
    }
  }

  var m = 1;
  if isDomainType(sizeType) then m = size.size;
  else if isIntegralType(sizeType) then m = size;
  
  var actualRatios = new map(int, real);
  for (k,v) in actualRatios.items() {
    if isNothingType(sizeType) then
      actualRatios[k] = v / trials:real;
    else
      actualRatios[k] = v / (trials*m): real;
  }
  
  var dom: domain(1) = {1..X.size};
  var ones: [dom] real = 1;

  var probabilities = if isNothingType(prob.type) then ones
                      else prob.reindex(1..X.size);

  // Get expected ratios
  var expectedRatios = new map(int, real);

  var total = (+ reduce probabilities):real;

  for i in dom{
    expectedRatios[i] += probabilities[i]:real / total;
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
      write('[', X, '], ');
      if !isNothingType(prob.type) then write('prob = ', prob, ', ');
      if !isNothingType(size.type) then writeln('size = ', size, ', ');
      writeln('replace = ', replace, ');');
      for value in actualRatios {
        writeln('value   expected   actual');
        writeln(value, '       ', expectedRatios[value:int],'        ', actualRatios[value]);
      }
    }
  }
}


/* This should really be part of the standard library! */
proc isClose(a: real, b: real, epsilon=0.05) {
  return abs(a - b) < epsilon;
}
