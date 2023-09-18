// Verify that `matches` produces at most one match
// when `text` is an empty string.

use Regex;

config param doTestEmpty = true;
config param doTestXY = true;
config param doTestSearch = true;
config param doTestSearchExtract = true;

var myString = "";

if doTestEmpty {
  writeln("matches on empty string");

  proc testEmpty(re:string) {
    // semicolon prevents warnings about end of line whitespace
    writeln("[", re, "]  ", (new regex(re)).matches(myString, 0, 6), ";");
  }

  testEmpty("");
  testEmpty("^");
  testEmpty("^[^\\s]");
  testEmpty("^x");
  testEmpty("^y");
  testEmpty("^z");
  testEmpty("$");
  testEmpty("^$");
  testEmpty(".*");
  testEmpty(".+");
  testEmpty(".");
  testEmpty("x");
  testEmpty("y");
  testEmpty("z");
  testEmpty("y|b");
}

myString = "xyab";

if doTestXY {

  writeln("\nmatches on non-empty string");

  proc testXY(re:string) {
    for mm in (new regex(re)).matches(myString, 0, 6) {
      var ss: string = myString[mm(0)];
      writeln("[", re, "]  ", mm, " <", ss, ">");
    }
    writeln("[", re, "]  done");
  }

  testXY("");
  testXY("^");
  testXY("^[^\\s]");
  testXY("^x");
  testXY("^y");
  testXY("^z");
  testXY("$");
  testXY("^$");
  testXY(".*");
  testXY(".+");
  testXY(".");
  testXY("x");
  testXY("y");
  testXY("z");
  testXY("y|b");
}

if doTestSearch {

  writeln("\nsearch");

  proc testSearch(re:string) {
    writeln("[", re, "]   ", (new regex(re)).search(myString));
  }

  testSearch("");
  testSearch("^");
  testSearch("^[^\\s]");
  testSearch("^x");
  testSearch("^y");
  testSearch("^z");
  testSearch("$");
  testSearch("^$");
  testSearch(".*");
  testSearch(".+");
  testSearch(".");
  testSearch("x");
  testSearch("y");
  testSearch("z");
  testSearch("y|b");

}

if doTestSearchExtract {

  writeln("\nsearch and extract");

  proc testSearchExtract(re:string) {
    var ss: string;
    // is this really needed? why not automatically get group 0 is the whole match?
    // this would be same as python
    // also doesnt match io patterns
    // Note: this is caused by `_handle_captures`, which always skips the first match
    // But we have tests that lock in the old behavior
    var re_ = "(" + re + ")";
    writeln("[", re_, "]   ", (new regex(re_)).search(myString, ss), " <", ss, ">");
  }

  testSearchExtract("");
  testSearchExtract("^");
  testSearchExtract("^[^\\s]");
  testSearchExtract("^x");
  testSearchExtract("^y");
  testSearchExtract("^z");
  testSearchExtract("$");
  testSearchExtract("^$");
  testSearchExtract(".*");
  testSearchExtract(".+");
  testSearchExtract(".");
  testSearchExtract("x");
  testSearchExtract("y");
  testSearchExtract("z");
  testSearchExtract("y|b");

}
