use IO, List, Map;

// A potentially overwrought solution that stored the entire state of
// the file system in hopes of using it for part 2.  No such luck...

config param largeFile = 100_000;

var grandtotal = 0;  // lame, serial

class Dir {
  var name: string;                     // name of this directory, for sanity
  var files: map(string, int);          // filename -> size map
  var subdirs: map(string, owned Dir);  // dir name -> contents map

  proc findSize(threshold): int {
    var total = 0;

    for filesize in files.values() do
      total += filesize;

    for name in subdirs do   // BUG: Can't use items() with owned values?!?
      total += subdirs.getBorrowed(name).findSize(threshold);

    if total < threshold then
      grandtotal += total;

    return total;
  }
}

var RootDir = new Dir("/");

var line: string;
readLine(line);
assert(line == "$ cd /\n");
readTerminal(RootDir.borrow());

RootDir.findSize(largeFile);
writeln(grandtotal);

// TODO: Make a method?  Or even an initializer?
proc readTerminal(cwd) {
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
	if !cwd.subdirs.contains(dirname) {
	  cwd.subdirs.add(dirname, new Dir(dirname));
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
  	  readTerminal(cwd.subdirs.getBorrowed(dirname));  // recurse
	}
      }

      // line:   12345 <filename>
      // token:  0     1
      otherwise {
        const token = line.split(" "),
	      size = token[0]:int;
	ref filename = token[1];
	    
	cwd.files.add(filename, size);
      }
    }
  }
}
