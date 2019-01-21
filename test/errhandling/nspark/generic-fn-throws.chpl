use ExampleErrors;
proc throwIfIntegral(x) throws where isIntegral(x) {
  throw new owned StringError("got an integer");
}

proc throwIfIntegral(x) {
  writeln(x);
}

try {
  throwIfIntegral(42.0);
  throwIfIntegral(42);
} catch err {
  writeln(err.message());
}
