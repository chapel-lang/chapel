// This version is like duplicates.chpl but uses a record instead of a tuple

use FileHashing;
use FileSystem;
use Sort;
use Time;

config const timing = false;

record HashAndPath {
  var hash: SHA256Hash;
  var path: string;
}

// stumbling block: no < for record
// stumbling block: comparator increases complexity
// stumbling block: need to know SHA256Hash has compare and/or <
record HashAndPathComparator {
  proc compare(a: HashAndPath, b: HashAndPath) {
    if a.hash < b.hash {
      return -1;
    }
    if a.hash > b.hash {
      return 1;
    }
    if a.path < b.path {
      return -1;
    }
    if a.path > b.path {
      return 1;
    }
    return 0;
  }
}

proc main(args: [] string) {
  var clock:Timer;
  var paths:domain(string);

  if timing {
    writeln("Gathering files");
  }
  clock.start();
  for arg in args[1..] {
    if isFile(arg) {
      paths += relativeRealPath(arg);
    } else if isDir(arg) {
      for path in findfiles(arg, recursive=true) {
        paths += relativeRealPath(path);
      }
    } else {
      writeln("Error: argument not handled ", arg);
    }
  }
  clock.stop();
  if timing {
    writeln("Gathering ", paths.size, " files took ", clock.elapsed(), " seconds");
  }

  // call computeFileHash to gather the hashes
  // computeFileHash is declared like this:
  //   computeFileHash(path: string): SHA256Hash throws
  // so it accepts a string argument and returns a SHA256Hash

  // hashAndPath is an array of tuples
  var hashAndPath:[1..paths.size] HashAndPath;

  // Set the path component in the array
  // stumbling block: can't forall over domain and convert to array
  for (rec, path) in zip(hashAndPath, paths) {
    rec.path = path;
  }

  if timing {
    writeln("Reading and hashing files");
  }
  clock.clear();
  clock.start();
  for rec in hashAndPath {
    rec.hash = computeFileHash(rec.path);
  }
  clock.stop();
  if timing {
    writeln("Reading and hashing files took ", clock.elapsed(), " seconds");
  }

  if timing {
    writeln("Sorting by hash");
  }
  clock.clear();
  clock.start();

  // stumbling block: need to know sort exists
  sort(hashAndPath, comparator=new HashAndPathComparator());

  clock.stop();
  if timing {
    writeln("Sorting by hash took ", clock.elapsed(), " seconds");
  }

  var i = 1;
  while i < paths.size {
    // Look for the group matching
    var next = i + 1;
    while next <= paths.size && hashAndPath[i].hash == hashAndPath[next].hash {
      next += 1;
    }
    // Now i..next-1 is the group matching
    if i < next-1 {
      writeln(hashAndPath[i].hash);
      for j in i..next-1 {
        assert(hashAndPath[i].hash == hashAndPath[j].hash);
        writeln("  ", hashAndPath[j].path);
      }
    }
    i = next;
  }
}
