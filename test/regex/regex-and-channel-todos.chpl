// Tests for #20864

use Regex, IO;

var f = openmem();
{ var w = f.writer(); w.write("xy"); }

proc testXY(re:string) {
  var fr = f.reader();
  for mm in fr.matches(compile(re), 0, 6) {
    var ss: string;
    fr.extractMatch(mm(0), ss);
    writeln("[", re, "]  ", mm, " <", ss, "> ", fr.offset());
  }
  writeln("[", re, "]  done ", fr.offset());
}


// This should matches once at the beginning of the channel.
// What should be the ending position in the channel?
// When this is fixed, merge it into empty-channel-matches.chpl
testXY("^"); 

writeln();

// This should match once at the end of the string, with byteOffset = 2.
// When this is fixed, merge it into empty-string-matches.chpl
writeln(compile("$").matches("xy"));
