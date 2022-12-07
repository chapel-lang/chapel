// A potentially overwrought solution that stored the entire state of
// the file system in hopes of using it for part 2.  No such luck...


// New features for the blog:

// classes
// - default initializers
// - secondary methods
// Maps
// - and map-of-owned specifically
// ref intents
// atomic vars
// Recursion (not particularly Chapel-specific
// assert()

use IO, Map;

// A class representing a directory and its contents
//
class Dir {
  var name: string;                     // name of this directory, for sanity
  var files: map(string, int);          // filename -> size map
  var subdirs: map(string, owned Dir);  // dir name -> contents map
}


// Kick things off by creating a root directory.
//
var RootDir = new Dir("/");

// Then read in the first line, making sure it's as expected.
//
var line: string;
readLine(line);
assert(line == "$ cd /\n");

// Then read in the rest of the disk's state.
//
RootDir.readContents();


// Now, compute the sum of all directory sizes less than 'dirThreshold'
config param dirThreshold = 100_000;
var sumOfSmall = 0;  // lame, serial
RootDir.findSize(dirThreshold, sumOfSmall);
writeln(sumOfSmall);

proc Dir.readContents() {
  var line: string;

  while readLine(line) {
    line = line.strip();  // remove the newline

    select line[..3] {  // categorize the command by the first 4 characters

      when "$ ls" {
        // no need to do anything, we'll just take care of the next lines
      }

      when "dir " {
        //
        // line:  dir <dirname>
        // token: 0   1
        // 
	const token = line.split(" ");
	ref dirname = token[1];

        // if we have not seen this directory before, add a new Dir entry
	if !subdirs.contains(dirname) {
	  subdirs.add(dirname, new Dir(dirname));
        }
	// otherwise, do nothing
      }

      when "$ cd" {
        //
        // line:  $ cd <dirname>
        // token: 0 1  2
        //
	const token = line.split(" ");
	ref dirname = token[2];

        if dirname == ".." {
	  return;  // pop us back up a level
	} else {
  	  subdirs.getBorrowed(dirname).readContents();  // recurse
	}
      }

      otherwise {  // This is a file
        //
        // line:   12345 <filename>
        // token:  0     1
	//
        const token = line.split(" "),
	      size = token[0]:int;
	ref filename = token[1];
	    
	files.add(filename, size);
      }
    }
  }
}


proc Dir.findSize(threshold, ref sum): int {
  var total = 0;

  for filesize in files.values() do
    total += filesize;

  for name in subdirs do   // BUG: Can't use items() with owned values?!?
    total += subdirs.getBorrowed(name).findSize(threshold, sum);

  if total < threshold then
    sum += total;

  return total;
}
