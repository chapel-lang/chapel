// A test of patterns that match an empty string.
// First, test them on an empty channel, then on a channel with "xx".

use Regex, IO;

config param doTestEmpty = true;
config param doTestXY = true;
config param doTestSearch = true;
config param doTestSearchExtract = true;

var f = openMemFile();

if doTestEmpty {
  writeln("matches on empty channel");

  proc testEmpty(re:string) {
    var fr = f.reader(locking=false);
    writeln("[", re, "]  ", fr.matches(new regex(re), 0, 6), " ", fr.offset());
  }

  // each of the following matches exactly once at offset 0, given an empty file
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


{ var w = f.writer(locking=false); w.write("xyab"); }

if doTestXY {

  writeln("\nmatches on non-empty channel");

  proc testXY(re:string) {
    var fr1 = f.reader(locking=false);
    for mm in fr1.matches(new regex(re), 0, 6) {
      var ss: string;
      fr1.extractMatch(mm(0), ss);
      writeln("[", re, "]  ", mm, " <", ss, "> ", fr1.offset());
    }
    writeln("[", re, "]  done ", fr1.offset());
  }

  testXY("");    // matches once before each char and once after the last char
  testXY("^"); // matches once at start of channel-->empty-channel-todo.future
  testXY("^[^\\s]");
  testXY("^x");
  testXY("^y");
  testXY("^z");
  testXY("$");   // matches once after the last char
  testXY("^$");  // no match, given that the file is not empty; final offset??
  testXY(".*");  // matches once everything at start and once like "$"
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
    var fr = f.reader(locking=false);
    writeln("[", re, "]   ", fr.search(new regex(re)), " ", fr.offset());
  }

  testSearch("");
  testSearch("^");
  testSearch("^[^\\s]");
  testSearch("^x");
  testSearch("^y");
  testSearch("^z");
  testSearch("$");
  testSearch("^$"); // what should be the offset after the search call here?
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
    var fr = f.reader(locking=false);
    var ss: string;
    writeln("[", re, "]   ", fr.search(new regex(re), ss), " <", ss, "> ", fr.offset());
  }

  testSearchExtract("");
  testSearchExtract("^");
  testSearchExtract("^[^\\s]");
  testSearchExtract("^x");
  testSearchExtract("^y");
  testSearchExtract("^z");
  testSearchExtract("$");
  testSearchExtract("^$"); // what should be the offset after the search call here?
  testSearchExtract(".*");
  testSearchExtract(".+");
  testSearchExtract(".");
  testSearchExtract("x");
  testSearchExtract("y");
  testSearchExtract("z");
  testSearchExtract("y|b");

}
