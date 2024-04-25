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

// in posix mode, 0f+? means the same as (0f+)?, and longer matches
// are preferred.
//
// in regular RE2 mode, +? means "one ore more, but prefer fewer"

// default
writeln("1");
extractMatch(new regex("0f+?"), "0fff1", "0f");
// false
writeln("2");
extractMatch(new regex("0f+?", posix=false), "0fff1", "0f");
// true
writeln("3");
extractMatch(new regex("0f+?", posix=true), "0fff1", "0fff");
