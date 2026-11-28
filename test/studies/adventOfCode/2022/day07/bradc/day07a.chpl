// WARNING: Not really cleaned up at all like part 1 was

use IO, List, Map;

var grandtotal = 0;  // lame, serial
var smallestSoFar = max(int);  // also lame

class Dir {
  var name: string;
  var subdirs: map(string, owned Dir);
  var files: map(string, int);

  proc findSize(): int {
    var total = 0;
    for (f,sz) in files.items() do  // could be items?
      total += sz;
    for name in subdirs do   // BUG: Can't use items() with owned values?!?
      total += subdirs.getBorrowed(name).findSize();
//    writeln("*>*>*>: ", name, " = ", total);
    if total < 100_000 {
      grandtotal += total;
    }
    return total;
  }

  proc findMinToDelete(amt): int {
    var total = 0;
    for (f,sz) in files.items() do  // could be items?
      total += sz;
    for name in subdirs do   // BUG: Can't use items() with owned values?!?
      total += subdirs.getBorrowed(name).findMinToDelete(amt);
//    writeln("*>*>*>: ", name, " = ", (total, amt, smallestSoFar));
    if total > amt && total < smallestSoFar then
      smallestSoFar = total;

    return total;
  }
}

var RootDir = new Dir("/");

var line: string;
readLine(line);
assert(line == "$ cd /\n");
readTerminal(RootDir.borrow());

var usedSpace = RootDir.findSize();
//writeln(grandtotal);
const totalDisk = 70_000_000;
const neededSpace = 30_000_000;
const freeSpace = totalDisk-usedSpace;
const amtToDelete = neededSpace - freeSpace;
//writeln((freeSpace, amtToDelete));

RootDir.findMinToDelete(amtToDelete);
writeln(smallestSoFar);

// TODO: Make a method?  Or even an initializer?
proc readTerminal(cwd) {
//  writeln("*** current directory is: ", cwd.name);
  while readLine(line) {
    line = line.strip();
    select line[..3] {
      when "$ ls" {
      /*
        while (line[0] != "$") {
	  readLine(line);
	  */
//        writeln("ls'd: ", line);
      }
      when "$ cd" {
//        writeln("cd'd: ", line);
	var token = line.split(" ");
	ref dirname = token[2];
	if dirname == ".." {
	  return;
	} else {
//          writeln("> Looking up ", token[2]);
  	  readTerminal(cwd.subdirs.getBorrowed(token[2]));
	}
      }
      when "dir " {
//        writeln("dir'd: ", line);
	var token = line.split(" ");
	if !cwd.subdirs.contains(token[1]) {
//	  writeln("> Inserting ", token[1]);
	  cwd.subdirs.add(token[1], new Dir(token[1]));
        } else {
//	  writeln("> Already knew about ", token[1]);
	}
      }
      otherwise {
        const words = line.split(" ");
//	writeln((words[0], words[1]));
	cwd.files.add(words[1], words[0]:int);
      }
    }
  }
}
