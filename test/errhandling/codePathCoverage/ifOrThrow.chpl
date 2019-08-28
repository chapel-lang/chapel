config const coinFlip = true;

class GoofyError: Error {
}

proc foo(): int throws {
  if (coinFlip) {
    return 42;
  } else {
    throw new owned Error();
  }
}

var x = foo();
writeln(x);
