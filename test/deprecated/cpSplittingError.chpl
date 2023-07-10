var s = "you cannot split this pie:🥧";

try {
  var slice = s[(s.find(":")+2)..];
} catch e: CodepointSplittingError {
  writeln(e.message());
} catch e {
  writeln("caught wrong error type: ", e);
}
