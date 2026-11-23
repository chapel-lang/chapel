//
// Load and execute code from a Chapel library.
//

use ChapelLibraryTestCommon;
use DynamicLibraryHelper;

config param numProcPtrsToConstructInExecutable = 0;

// Call this setup function in both the library and executable.
perProgramSetupInModuleInit(numProcPtrsToConstructInExecutable);

// TODO: Need to support the situation where a loaded program has more
// global variables than the root program - today the program crashes with
// an internal error from the runtime.
var tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9, tmp10: int;

inline proc loadedTestName(ref lib, idx: int) {
  type testNameType = proc(x: int): c_ptrConst(c_char);
  const testNameProc = lib.loadLocally('testName', testNameType);
  const ptr = testNameProc(idx);
  const ret = try! string.createCopyingBuffer(ptr);
  return ret;
}

inline proc numberOfLoadedTests(ref lib) {
  type numTestsType = proc(): int;
  const numTestsProc = try! lib.loadLocally('numberOfTestsToRun', numTestsType);
  const ret = numTestsProc();
  return ret;
}

proc runTestsInLibrary(ref lib: dynamicLibraryHelper) {
  // Get the number of tests to run.
  const numTests = numberOfLoadedTests(lib);
  writeln('Executing ', numTests, ' tests in library...');
  writeln();

  for i in 0..<numTests {
    // Get the test name, this will be dynamically loaded.
    const testName = loadedTestName(lib, i);

    printTestHeader(testName);

    // Fetch the test locally.
    type testType = proc(): bool;
    const test = try! lib.loadLocally(testName, testType);
    assert(test != nil);

    // Run the test. TODO: If I assign this into a bool it doesn't run.
    test();

    // Print out newline just to space things out.
    writeln();
  }
}

// TODO: Returning procedures, then call me below...
proc testAddProcFactory(ref lib: dynamicLibraryHelper) {
  type fnType = proc(): proc(a: int, b: int): int;
  const fn = try! lib.load('returnAddProc', fnType);

  // NOTE: While 'addProcFactory' has external linkage, 'addProc' doesn't.
  const addProc = fn();

  for i in 1..hi do {
    const n1 = addProc(i, i);
    const n2 = i + i;
    assert(matches(n1, n2));
  }
}

proc testPassingArgs(ref lib: dynamicLibraryHelper) {
  type fnType = proc(a: int, b: int, c: int): void;
  const fn = try! lib.load('writelnThreeInt64', fnType);
  fn(3, 1, 4);
}

proc testReturningVal0(ref lib: dynamicLibraryHelper) {
  type fnType = proc(): int;
  const fn = try! lib.load('returnMeaningOfLife', fnType);
  const n = fn();
  writeln(n);
}

proc testReturningVal1(ref lib: dynamicLibraryHelper) {
  writeln('SKIPPING: Module-level string constants are broken');
  return;

  type fnType = proc(): c_ptrConst(c_char);
  const fn = try! lib.load('returnStringPtr', fnType);
  const ptr = fn();
  const str = try! string.createBorrowingBuffer(ptr);
  writeln(str);
}

proc testAddTwoInt64(ref lib: dynamicLibraryHelper) {
  type fnType = proc(a: int, b: int): int;
  const fn = try! lib.load('addTwoInt64', fnType);
  const n = fn(2, 2);
  assert(n == 4);
}

// In the first variation we load outside of the coforall.
proc testCoforallCallingSerial0(ref lib: dynamicLibraryHelper) {
  type fnType = proc(x: int): void;
  const fnToWrite = try! lib.load('writeInt64', fnType);

  var counter: atomic int;
  const hi = 16;

  // This is a really goofy loop that just serves to spawn a bunch of tasks.
  while counter.read() != hi do
    coforall i in 0..<hi do
      if i == counter.read() {
        fnToWrite(i);
        write(' ');
        counter.add(1);
      }

  writeln();
}

// In the second variation we load inside of the coforall.
proc testCoforallCallingSerial1(ref lib: dynamicLibraryHelper) {
  var counter: atomic int;

  // Just a goofy summation '(k*(k+1))/2' but doubled...
  coforall i in 1..hi with (ref lib) {
    // At least one first-time load under contention. Then fast-path access.
    type fnType = proc(a: int, b: int): int;
    const fn = try! lib.load('addCoforallCallingSerial1', fnType);

    const n = fn(i, i);
    counter.add(n);
  }

  writeln(counter);
}

proc testCoforallCallingCoforall(ref lib: dynamicLibraryHelper) {
  const hi = 16;
  var arr: [1..hi] int;

  coforall i in 1..hi with (ref arr, ref lib) do {
    type fnType = proc(n: int): int;
    const fn = try! lib.load('coforallComputeSummation', fnType);
    arr[i] = fn(i);
  }

  writeln(arr);
}

// TODO: Support getting name from procedure pointer.
const testArray = [
  ('testPassingArgs', testPassingArgs),
  ('testReturningVal0', testReturningVal0),
  ('testReturningVal1', testReturningVal1),
  ('testCoforallCallingSerial0', testCoforallCallingSerial0),
  ('testCoforallCallingSerial1', testCoforallCallingSerial1),
  ('testCoforallCallingCoforall', testCoforallCallingCoforall),
];

proc runTestsInExecutable(ref lib: dynamicLibraryHelper) {
  const numTests = testArray.size;

  writeln('Executing ', numTests, ' tests in executable...');
  writeln();

  for (testName, testProc) in testArray {
    printTestHeader(testName);
    testProc(lib);
  }
}

proc main() {
  var lib = new dynamicLibraryHelper(chapelLibraryPath);

  runTestsInLibrary(lib);

  runTestsInExecutable(lib);
}
