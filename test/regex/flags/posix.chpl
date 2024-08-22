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
writeln("checking 0f+?, default");
extractMatch(new regex("0f+?"), "0fff1", "0f");
// false
writeln("checking 0f+?, posix=false");
extractMatch(new regex("0f+?", posix=false), "0fff1", "0f");
// true
writeln("checking 0f+?, posix=true");
extractMatch(new regex("0f+?", posix=true), "0fff1", "0fff");
writeln();

// check that posix mode implies multiline mode
writeln("checking multiline mode, default");
extractMatch(new regex("^a"), "bc\na\n", "<not found>");
writeln("checking multiline mode, posix=false");
extractMatch(new regex("^a", posix=false), "bc\na\n", "<not found>");
writeln("checking multiline mode, posix=true");
extractMatch(new regex("^a", posix=true), "bc\na\n", "a");
writeln();

// check that posix mode activates leftmost longest match
writeln("replace bb with |b->a, default");
writeln("bb".replace(new regex("|b", posix=false), "a"));
writeln("replace bb with |b->a, posix=false");
writeln("bb".replace(new regex("|b", posix=false), "a"));
writeln("replace bb with |b->a, posix=true");
writeln("bb".replace(new regex("|b", posix=true), "a"));
writeln();
