use Random, Map;

config const debug = false;

proc main() {
  var pcg = createRandomStream(real, algorithm=RNG.PCG, seed=42);
  runTests(pcg);
  var pcgInt = createRandomStream(int, algorithm=RNG.PCG, seed=420);
  runTests(pcgInt);
}

proc runTests(stream) {
  testDomain(stream, {1..1}, trials=10);
  testDomain(stream, {1..3});


  // offset & strided domain
  var strideDom = {2..10 by 2};
  testDomain(stream, strideDom, size=20);
  testDomain(stream, strideDom, prob=[0, 1, 2, 3, 4], size=20);

  // prob array
  testDomain(stream, {1..2}, prob=[0, 1], trials=10);
  testDomain(stream, {1..2}, prob=[0.1, 0.9]);
  testDomain(stream, {1..3}, prob=[0.1, 0.4, 0.5]);

  // replace=false
  testDomain(stream, {1..4}, size=4, replace=false, trials=1);

  // domain-type size
  var A = {1..4};
  var p = [0.1, 0.2, 0.3, 0.4];
  testDomain(stream, A, prob=p, size={1..4});
  testDomain(stream, A, prob=p, size={1..4, 1..3});
  testDomain(stream, A, prob=p, size={1..4, 1..3, 1..2});
}

proc testDomain(stream, dom: domain, size:?sizeType=none, replace=true, prob:?probType=none, trials=10000) throws {
  var counts = new map(int, int);

  // Collect statistics
  if isNothingType(probType) {
    if isNothingType(sizeType) {
      for 1..trials {
        var c = stream.choice(dom, replace=replace);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choice(dom, size=size, replace=replace);
        for c in cs do counts[c] += 1;
      }
    }
  } else {
    if isNothingType(sizeType) {
      for 1..trials {
        var c = stream.choice(dom, prob=prob, replace=replace);
        counts[c] += 1;
      }
    } else {
      for 1..trials {
        var cs = stream.choice(dom, prob=prob, size=size, replace=replace);
        for c in cs do counts[c] += 1;
      }
    }
  }


  if debug {
    writeln('Counts for domain: ', dom);
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

  var Dom: domain(1) = {1..dom.size};
  var ones: [Dom] real = 1;

  var probabilities = if isNothingType(prob.type) then ones
                      else prob.reindex(1..dom.size);
  
  // Get expected ratios
  var uniqueValues: domain(int);
  var expectedRatios = new map(int, real);

  var total = (+ reduce probabilities):real;

  for i in Dom{
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
      write('[', dom, '], ');
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
