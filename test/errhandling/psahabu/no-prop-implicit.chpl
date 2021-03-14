use ExampleErrors;

proc noPropError() {
  writeln("should not propagate");
  throwAnError();
  writeln("fail: propagated");
}

writeln("calling noPropError");
noPropError();
