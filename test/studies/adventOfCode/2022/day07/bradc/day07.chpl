// classes
// - default initializers
// - secondary methods
// Maps
// - and map-of-owned specifically
// ref intents
// atomic vars
// Recursion (not particularly Chapel-specific

use IO, Map;

// A potentially overwrought solution that stored the entire state of
// the file system in hopes of using it for part 2.  No such luck...

config param dirThreshold = 100_000;

class Dir {
  var name: string;                     // name of this directory, for sanity
  var files: map(string, int);          // filename -> size map
  var subdirs: map(string, owned Dir);  // dir name -> contents map
}

// Get the stte of the world by creating our root directory...
var RootDir = new Dir("/");

// ...reading the first line and making sure it's correct...
var line: string;
readLine(line);
assert(line == "$ cd /\n");

// ...then reading in the rest of the world's state.
RootDir.readContents();


// Now compute the sum of all directory sizes less than 
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


      // line:  dir <dirname>
      // token: 0   1
      // 
      when "dir " {  // if this is a directory
	const token = line.split(" ");
	ref dirname = token[1];

        // if we have not seen this directory before, add a new Dir entry
	if !subdirs.contains(dirname) {
	  subdirs.add(dirname, new Dir(dirname));
        }
	// otherwise, do nothing
      }

      // line:  $ cd <dirname>
      // token: 0 1  2
      //
      when "$ cd" {
	const token = line.split(" ");
	ref dirname = token[2];

        if dirname == ".." {
	  return;  // pops us back up a level
	} else {
  	  subdirs.getBorrowed(dirname).readContents();  // recurse
	}
      }

      // line:   12345 <filename>
      // token:  0     1
      otherwise {
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
