use SomeModuleContainingTests;
use UnitTest;

proc celsius2fahrenheit(x) {
  // we should be returning "(x: real * 9/5)+32"
  return (x * 9/5)+32;
}

proc test_temperature(test: borrowed Test) throws {
  // we were expecting 98.6 but since we missed typecasting
  // the above function returned 98.
  test.assertFalse(celsius2fahrenheit(37) == 98);
}

UnitTest.main();
