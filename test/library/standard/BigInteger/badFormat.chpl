// Tests that we throw a BadFormatError appropriately
use BigInteger;
use OS;

try {
  var bad = new bigint("edy102920");
} catch e: BadFormatError {
  writeln("correctly threw BadFormatError");
  writeln(e.message());
} catch e : Error {
  writeln("uh oh, caught other error!");
  writeln(e.message());
}
