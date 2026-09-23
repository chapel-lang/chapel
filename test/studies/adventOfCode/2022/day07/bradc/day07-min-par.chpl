// A potentially overwrought solution that stored the entire state of
// the file system in hopes of using it for part 2.  No such luck...


// New features used here, not seen on the blog yet:

// classes
// - default initializers
// - secondary methods
// select statements
// atomic vars
// Recursion (not particularly Chapel-specific)
// assert()

use IO, List;

// A class representing a directory and its contents
//
class Dir {
  var filesizes: list(int);      // list of filenames
  var subdirs: list(owned Dir);  // list of subdirs
}


// Kick things off by creating a root directory.
//
var RootDir = new Dir();


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
var sumOfSmall: atomic int = 0;

RootDir.findSize(dirThreshold, sumOfSmall);
writeln(sumOfSmall.read());

//
// Routine for reading in a directory's contents via terminal session commands
//
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

        /* borrow idea from Daniel and do nothing here, moving work to
           'cd' instead */
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
          var newdir = new Dir();
          newdir.readContents();
          subdirs.append(newdir);
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
   
        filesizes.append(size);
      }
    }
  }
}


//
// Routine for adding up directory sizes less than threshold
//
proc Dir.findSize(threshold, sum): int {
  var total = 0;

  forall filesize in filesizes with (+ reduce total) do
    total += filesize;

  forall subdir in subdirs with (+ reduce total) do  // loops over values
    total += subdir.findSize(threshold, sum);

  if total < threshold then
    sum.add(total);  // should be sum += total -- see existing issue

  return total;
}
