config const tryLocale = 0;
config var globalNonref = 10;
var globalVar: int;
var globalByRef: int;
var beginProceed$: sync int;

proc testDriver(): int {
  var localVar: int;
  var localByRef: int;
  proc testNested(ref globalArg: int, ref localArg: int): void {
    begin ref(globalVar,globalArg,localVar,localArg) {
      beginProceed$;
      globalVar = 200;
      globalArg = 3000;
      localVar = 40000;
      localArg = 500000;
    }
  }
  sync {
    testNested(globalByRef, localByRef);
    beginProceed$ = 1;
  }

  return localVar + localByRef;
}

const driverResult = testDriver();
const totalResult = globalNonref + globalVar + globalByRef + driverResult;
writeln(totalResult);
