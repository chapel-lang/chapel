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

writeln("checking dotall mode, default");
extractMatch(new regex("c.a"), "bc\na\n", "<not found>");
writeln("checking dotall mode, dotAll=false");
extractMatch(new regex("c.a", dotAll=false), "bc\na\n", "<not found>");
writeln("checking dotall mode, dotAll=true");
extractMatch(new regex("c.a", dotAll=true), "bc\na\n", "c\na");
