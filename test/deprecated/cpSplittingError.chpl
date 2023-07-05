var s = "you cannot split this pie: 🥧";

try {
  var x = s[(s.find(": ")+1)..];
} catch e: CodepointSplittingError {
  writeln(e.message());
} catch e {
  writeln("caught wrong error type!");
}
