config const coinFlip = true;

class GoofyError: Error {
}

proc foo(): int throws {
  if (coinFlip) {
    return 0;
  } else {
    throw new owned Error();
  }
}

proc bar() throws {
  try {
    if coinFlip then
      return foo();
  } catch e: GoofyError {
    throw e;
  } catch {
    throw new owned GoofyError();
  }
}
  

var x = bar();

    