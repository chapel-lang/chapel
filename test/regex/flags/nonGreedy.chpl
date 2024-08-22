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
extractMatch(new regex("0f*"), "0fff1", "0fff");
extractMatch(new regex("0f+"), "0fff1", "0fff");
// false
writeln("2");
extractMatch(new regex("0f*", nonGreedy=false), "0fff1", "0fff");
extractMatch(new regex("0f+", nonGreedy=false), "0fff1", "0fff");
// true
writeln("3");
extractMatch(new regex("0f*", nonGreedy=true), "0fff1", "0");
extractMatch(new regex("0f+", nonGreedy=true), "0fff1", "0f");

// nonGreedy is ignored for posix regex
writeln("4");
extractMatch(new regex("0f*", posix=true), "0fff1", "0fff");
extractMatch(new regex("0f+", posix=true), "0fff1", "0fff");
writeln("5");
extractMatch(new regex("0f*", posix=true, nonGreedy=false), "0fff1", "0fff");
extractMatch(new regex("0f+", posix=true, nonGreedy=false), "0fff1", "0fff");
writeln("6");
extractMatch(new regex("0f*", posix=true, nonGreedy=true), "0fff1", "0fff");
extractMatch(new regex("0f+", posix=true, nonGreedy=true), "0fff1", "0fff");
