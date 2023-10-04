import IO;
import SortedMap.sortedMap as map;
import Regex.regex;

// Read lines from the input file using an iterator
// Don't use this. Use fileReader.matches instead
iter readWarningLines(){
  var fileName, lineNumber, warning : string;
  // A pattern of a typical warning line
  // It consists of a file name (ending in .chpl)
  // Then a line number
  // And finally the error/warning message
  // These are seperated by colons (:)
  // Ex: ra-unordered-atomics.chpl:75: warning: 'dmapped' keyword is unst....
  const warningPattern = "%/(.*).chpl:(\\d+): (.*)\n/";

  while IO.readf(warningPattern, fileName, lineNumber, warning) {
    yield warning;
  }
}

proc countUniqueWarnings(){
  var uniqueWarnings  = new map(string, int);
  const warningRegex = new regex(".*.chpl:\\d+: (.*)\n"); // The  (.*) is a capture group
  var warning : string;
  for (fullMatch, warningMatch) in IO.stdin.matches(warningRegex, captures=1) {
    IO.stdin.extractMatch(warningMatch, warning);
    // Check if the string mentions that something is unstable
    // If so, add it to the map
    if warning.find("unstable") == -1 then
      continue;
    if uniqueWarnings.contains(warning) {
      uniqueWarnings[warning] += 1;
    } else {
      uniqueWarnings[warning] = 1;
    }
  }
  return uniqueWarnings;
}


proc prettyPrintMap(map){
  for key in map {
    if map[key] < 2 then
      writeln(map[key], " instance of \"", key, "\"");
    else writeln(map[key], " instances of \"", key, "\"");
  }
}

writeln("Ready...");
const uniqueWarnings = countUniqueWarnings();
prettyPrintMap(uniqueWarnings);
