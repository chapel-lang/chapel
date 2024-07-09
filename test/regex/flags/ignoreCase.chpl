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
writeln("1");
extractMatch(new regex("0F+"), "0fff1", "<not found>");
// false
writeln("2");
extractMatch(new regex("0F+", ignoreCase=false), "0fff1", "<not found>");
// true
writeln("3");
extractMatch(new regex("0F*", ignoreCase=true), "0fff1", "0fff");
