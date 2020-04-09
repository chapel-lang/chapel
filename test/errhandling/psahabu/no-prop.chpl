use ExampleErrors;

proc noPropError() {
  writeln("should not propagate");
  try! throwAnError();
  writeln("fail: propagated");
}

writeln("calling noPropError");
noPropError();
