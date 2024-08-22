const n = 10;

config const useReduceEquals = false;
config const useInitVal = false;

on here.gpus[0] {
  var A: [1..n] int;

  foreach (i,a) in zip(A.domain, A) do a = i;

  var expectedSumVar, expectedMinVar, expectedMaxVar: int;
  for a in A do expectedSumVar +=  a;

  var sumVar=0, minVar=max(int), maxVar=min(int): int;
  if useInitVal {
    sumVar = 100;
    minVar = -100;
    maxVar = n+100;

    expectedSumVar += sumVar;
    expectedMinVar = minVar;
    expectedMaxVar = maxVar;
  }
  else {
    expectedMinVar = A.domain.low;
    expectedMaxVar = A.domain.high;
  }

  forall a in A with (+ reduce sumVar, min reduce minVar, max reduce maxVar) {
    sumVar += a;
    if useReduceEquals {
      minVar reduce= a;
      maxVar reduce= a;
    }
    else {
      if a<minVar then minVar = a;
      if a>maxVar then maxVar = a;
    }
  }

  validate("sum", sumVar, expectedSumVar);
  validate("min", minVar, expectedMinVar);
  validate("max", maxVar, expectedMaxVar);
}

proc validate(name, actual, expected) {
  if actual != expected {
    writef("Incorrect result for %s\n", name);
    writef("\tActual=%i, Expected=%i\n", actual, expected);
  }
  else {
    writef("Correct result for %s\n", name);
  }
}
