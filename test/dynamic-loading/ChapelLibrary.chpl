module ChapelLibrary {
  use ChapelLibraryTestCommon;

  config param numProcPtrsToConstructInLibrary = 0;

  // Call this setup function in both the library and executable.
  perProgramSetupInModuleInit(numProcPtrsToConstructInLibrary);

  // Execute on current locale, print using 'printf'.
  export proc testPrintfLocally(): bool {
    printfGreetingsHere();
    return true;
  }

  // Execute on all locales using an 'on' statement. To print use 'printf'.
  export proc testPrintfOnAllLocales(): bool {
    for loc in Locales do on loc do printfGreetingsHere();
    return true;
  }

  // Execute on all locales but use a 'coforall' loop.
  export proc testCoforallFanOutAtomicIncrement(): bool {
    var counter: atomic int;
    coforall loc in Locales do on loc do counter.add(1);
    return matches(numLocales, counter.read(), print=true);
  }

  export proc testWritelnLocally() {
    writelnGreetingsHere();
    return true;
  }

  export proc testWritelnOnAllLocales() {
    for loc in Locales do on loc do writelnGreetingsHere();
    return true;
  }

  export proc testEnableWritelnByDefault() {
    ChapelLibraryTestCommon.printUsingWriteln = true;
    return true;
  }

  export proc testForallLocalAtomicIncrement() {
    var counter: atomic int;
    forall i in 0..<hi do counter.add(1);
    return matches(hi, counter.read(), print=true);
  }

  const testNameArray = [
    'testPrintfLocally',
    'testPrintfOnAllLocales',
    'testCoforallFanOutAtomicIncrement',
    'testWritelnLocally',
    'testWritelnOnAllLocales',
    'testEnableWritelnByDefault',
    'testForallLocalAtomicIncrement',
  ];

  // Executable will call to get the number of tests to run.
  export proc numberOfTestsToRun(): int {
    return testNameArray.size;
  }

  // Executable will call to get the test names.
  export proc testName(idx: int) {
    assert(idx >= 0 && idx < testNameArray.size);
    return testNameArray[idx].c_str();
  }

  //
  // HELPER FUNCTIONS (to be called by executable)
  //

  export proc writelnThreeInt64(a: int, b: int, c: int) {
    writeln(a, '.', b, '.', c);
  }

  export proc writeInt64(x: int) do write(x);
  export proc addTwoInt64(a: int, b: int) do return a + b;
  export proc addCoforallCallingSerial1(a: int, b: int) do return a + b;
  export proc returnMeaningOfLife(): int do return 42;

  // TODO: This causes everything to crash. Running some 'on' statements?
  // const testString = 'This is a long test string!';
  // export proc returnStringPtr() do return testString.c_str();

  // A very inefficient way to compute a summation. The point of this is to
  // spawn a lot of tasks to test heavy contention within the tasking layer.
  export proc coforallComputeSummation(n: int): int {
    if n <= 0 then return 0;
    var counter: atomic int;
    coforall i in 1..n do counter.add(i);
    return counter.read();
  }

  /*
  TODO: Returning procedures, and linkage. Currently not "externable".
  export proc returnAddProc(): myAddProc.type {
    return addTwoInt64;
  }
  */
}
