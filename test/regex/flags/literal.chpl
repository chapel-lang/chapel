use Regex;

proc extractMatch(re, input, expect) {
  var match = re.search(input);
  var result:string;
  if match {
    result = input[match];
  } else {
    result = "<not found>";
  }

  writeln("matched '", result, "'");
  assert(result == expect);
}

// default
writeln("checking 0f+, default");
extractMatch(new regex("0f+"), "0fff1", "0fff");
// false
writeln("checking 0f+, literal=false");
extractMatch(new regex("0f+", literal=false), "0fff1", "0fff");
// true
writeln("checking 0f+, literal=true");
extractMatch(new regex("0f+", literal=true), "0fff1", "<not found>");
extractMatch(new regex("0f+", literal=true), "00f++", "0f+");

// check that the various flags are ignored for literal mode
writeln("checking 0f+, literal=true + other flags");
extractMatch(new regex("0f+", literal=true, multiLine=true, nonGreedy=true), "00f++", "0f+");
extractMatch(new regex("0f+", literal=true, posix=true), "00f++", "0f+");
