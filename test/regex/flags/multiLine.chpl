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

writeln("checking multiline mode, default");
extractMatch(new regex("^a"), "bc\na\n", "<not found>");
writeln("checking multiline mode, multiLine=false");
extractMatch(new regex("^a", multiLine=false), "bc\na\n", "<not found>");
writeln("checking multiline mode, multiLine=true");
extractMatch(new regex("^a", multiLine=true), "bc\na\n", "a");
