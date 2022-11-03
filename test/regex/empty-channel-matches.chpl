// A test of patterns that match an empty string.
// First, test them on an empty channel, then on a channel with "xx".

use Regex, IO;

var f = openmem();

proc testEmpty(re:string) {
  var fr = f.reader();
  writeln("[", re, "]  ", fr.matches(compile(re), 0, 6), " ", fr.offset());
}

// each of the following matches exactly once at offset 0, given an empty file
testEmpty("");
testEmpty("^");
testEmpty("$");
testEmpty("^$");
testEmpty(".*");

writeln();

{ var w = f.writer(); w.write("xy"); }

proc testXY(re:string) {
  var fr1 = f.reader();
  for mm in fr1.matches(compile(re), 0, 6) {
    var ss: string;
    fr1.extractMatch(mm(0), ss);
    writeln("[", re, "]  ", mm, " <", ss, "> ", fr1.offset());
  }
  writeln("[", re, "]  done ", fr1.offset());
}

testXY("");    // matches once before each char and once after the last char
//testXY("^"); // matches once at start of channel-->empty-channel-todo.future
testXY("$");   // matches once after the last char
testXY("^$");  // no match, given that the file is not empty; final offset??
testXY(".*");  // matches once everything at start and once like "$"

// check these non-empty patterns as well
testXY(".");
testXY("x");
testXY("y");

writeln();

proc testSearch(re:string) {
  var fr = f.reader();
  writeln("[", re, "]   ", fr.search(compile(re)), " ", fr.offset());
}

testSearch("");
testSearch("^");
testSearch("$");
testSearch("^$"); // what should be the offset after the search call here?
testSearch(".*");
testSearch(".");
testSearch("x");
testSearch("y");

writeln();
var frx = f.reader();
const rex = "^$";
// how should the offset progress here? see also testSearch("^$") above
writeln("[", rex, "]   ", frx.search(compile(rex)), " ", frx.offset());
writeln("[", rex, "]   ", frx.search(compile(rex)), " ", frx.offset());
