use UnorderedCopy;

config const size = here.maxTaskPar * 100;
config const printResults = false;

// Test unordered PUT where src is temp stack variable 
proc testPUT(type copyType) {
  var A: [1..size] copyType;

  on Locales[numLocales-1] {
    forall i in 1..size do
      unorderedCopy(A[i], (i%2):copyType);
  }

  forall i in 1..size do
    assert(A[i] == (i%2):copyType);

  if printResults {
    writeln(copyType:string + " unordered PUT local");
    writeln(A);
  }
}

// Test unordered PUT where src is ref to array element
proc testPUTArr(type copyType) {
  var A: [1..size] copyType;

  on Locales[numLocales-1] {
    var B: [1..size] copyType;
    forall i in 1..size do
      B[i] = (i%2):copyType;

    forall i in 1..size do
      unorderedCopy(A[i], B[i]);
  }

  forall i in 1..size do
    assert(A[i] == (i%2):copyType);

  if printResults {
    writeln(copyType:string + " unordered PUT array");
    writeln(A);
  }
}

// Test unordered GET 
proc testGET(type copyType) {
  var A: [1..size] copyType;
  forall i in 1..size do
    A[i] = (i%2):copyType;

  on Locales[numLocales-1] {
    var B: [1..size] copyType;
    forall i in 1..size do
      unorderedCopy(B[i], A[i]);

    forall i in 1..size do
      assert(B[i] == (i%2):copyType);

    if printResults {
      writeln(copyType:string + " unordered GET");
      writeln(B);
    }
  }
}

// Test unordered PUT where different types will buffered adjacently
proc testPUTComb() {
  var AB:  [1..size] bool;
  var AI8: [1..size] int(8);
  var AI:  [1..size] int;
  var AR:  [1..size] real;
  var AC:  [1..size] complex; 

  on Locales[numLocales-1] {
    forall i in 1..size {
      unorderedCopy(AB[i]  , (i%2):bool);
      unorderedCopy(AI8[i] , (i%max(int(8))):int(8));
      unorderedCopy(AI[i]  , i);
      unorderedCopy(AR[i]  , i:real);
      unorderedCopy(AC[i]  , i:complex);
    }
  }

  forall i in 1..size {
    assert(AB[i]  == (i%2):bool);
    assert(AI8[i] == (i%max(int(8))):int(8));
    assert(AI[i]  == i);
    assert(AR[i]  == i:real);
    assert(AC[i]  == i:complex);
  }

  if printResults {
    writeln("Unordered PUT combo");
    writeln(AB);
    writeln(AI8);
    writeln(AI);
    writeln(AR);
    writeln(AC);
  }
}

// Test unordered GET where different types will buffered adjacently
proc testGETComb() {
  var AB:  [1..size] bool;
  var AI8: [1..size] int(8);
  var AI:  [1..size] int;
  var AR:  [1..size] real;
  var AC:  [1..size] complex;

  forall i in 1..size {
    AB[i]  = (i%2):bool;
    AI8[i] = (i%max(int(8))):int(8);
    AI[i]  = i;
    AR[i]  = i:real;
    AC[i]  = i:complex;
  }

  on Locales[numLocales-1] {
    var BB:  [1..size] bool;
    var BI8: [1..size] int(8);
    var BI:  [1..size] int;
    var BR:  [1..size] real;
    var BC:  [1..size] complex;

    forall i in 1..size {
      unorderedCopy(BB[i] , AB[i]);
      unorderedCopy(BI8[i], AI8[i]);
      unorderedCopy(BI[i] , AI[i]);
      unorderedCopy(BR[i] , AR[i]);
      unorderedCopy(BC[i] , AC[i]);
    }

    forall i in 1..size {
      assert(BB[i]  == (i%2):bool);
      assert(BI8[i] == (i%max(int(8))):int(8));
      assert(BI[i]  == i);
      assert(BR[i]  == i:real);
      assert(BC[i]  == i:complex);
    }

    if printResults {
      writeln("Unordered GET combo");
      writeln(AB);
      writeln(AI8);
      writeln(AI);
      writeln(AR);
      writeln(AC);
    }
  }
}


testPUT(bool);
testPUT(int(8));
testPUT(int);
testPUT(real);
testPUT(complex);
if printResults then writeln();

testPUTArr(bool);
testPUTArr(int(8));
testPUTArr(int);
testPUTArr(real);
testPUTArr(complex);
if printResults then writeln();

testGET(bool);
testGET(int(8));
testGET(int);
testGET(real);
testGET(complex);
if printResults then writeln();

testPUTComb();
if printResults then writeln();
testGETComb();
