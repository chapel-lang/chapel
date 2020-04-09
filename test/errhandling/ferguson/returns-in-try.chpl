class MyError : Error {
}
class OtherError : Error {
}

config const throwOnTest = 0;

proc throwOrReturn(testNumber:int): int throws {
  if testNumber == throwOnTest then
    throw new owned MyError();
  
  return testNumber;
}

proc trybang() {
  try! {
    return throwOrReturn(1);
  }
}
proc trybang2(): int {
  try! {
    return throwOrReturn(2);
  }
}
proc tryexhaustive() {
  try {
    return throwOrReturn(3);
  } catch {
    return 30;
  }
}
proc trybangcatch(): int {
  try! {
    return throwOrReturn(4);
  } catch e: MyError {
    return 40;
  }
}
proc trybangcatch2(): int {
  try! {
    return throwOrReturn(5);
  } catch e: OtherError {
    return 100;
  } catch e: MyError {
    return 50;
  }
}
proc trybangcatch3(): int {
  try! {
    return throwOrReturn(6);
  } catch e: OtherError {
    return 100;
  }
}



writeln(trybang());
writeln(trybang2());
writeln(tryexhaustive());
writeln(trybangcatch());
writeln(trybangcatch2());
writeln(trybangcatch3());
