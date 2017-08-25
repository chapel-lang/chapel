/*
 A unix like grep function implemented in chapel.
*/

use Path;
use IO;
use Regexp;
use FileSystem;
use SysBasic;
use Time;

// To keep track of line numbers and speed parallel speedup
var lineNum: int = 1;
var clock: Timer;

// Main
proc main(args: [] string) {

  if args.size < 3 {
    // clock.start();
    parallelGrep(args[1]);
    // clock.stop();
    // writeln(clock.elapsed());  uncomment for runtime
  }
  if args.size == 3 {
    fileGrep(args[1], args[2]);
  }
}


/*
 Simple version: needs
   - file name
   - string to be searched for
*/
proc fileGrep(toFind: string, fname: string) {

  var toRead: file;
  try! {
    toRead = open(fname, iomode.r);
  }
  var r = toRead.reader();

  // Use regex library to compile into searchable regex
  var regEx = compile(toFind);
  var line:string;
  var lineNum = 1;

  // Search while handling errors
  while (true) {
    try {
      r.readline(line);
    } catch {
      break;
    }

    if line.search(regEx) {
      writeln("Found " + toFind + " at " + lineNum + " in " + fname);
    }
    lineNum += 1;
  }
  r.close();  // close file
}



/*
 Parallel grep: needs
   - string to be searched for only
 Runs in parallel across any files in the current working directory.
*/
proc parallelGrep(tofind: string) {

  // Seaches current working directory ignoring anthing but UTF-8
  // encoded characters
  var files = FileSystem.findfiles(Path.curDir);
  forall file in files {
    fileGrep(tofind, file);
  }
}
