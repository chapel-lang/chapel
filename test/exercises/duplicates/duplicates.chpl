use FileSystem;
use Sort;
use Time;
use FileHashing;

config const timing = false;

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

  // sizeHashAndPath is an array of tuples
  var sizeHashAndPath:[1..paths.size] (int, SHA256Hash, string);
  // Set the path component in the array
  for (tup, path) in zip(sizeHashAndPath, paths) {
    tup(3) = path;
  }

  if timing {
    writeln("Reading and hashing files");
  }
  clock.clear();
  clock.start();
  forall (size, hash, path) in sizeHashAndPath {
    size = getFileSize(path);
    hash = computeFileHash(path);
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

  sort(sizeHashAndPath);

  clock.stop();
  if timing {
  writeln("Sorting by hash took ", clock.elapsed(), " seconds");
  }

  var i = 1;
  while i < paths.size {
    // Look for the group matching
    var next = i + 1;
    while next <= paths.size && sizeHashAndPath[i](2) == sizeHashAndPath[next](2) {
      next += 1;
    }
    // Now i..next-1 is the group matching
    if i < next-1 {
      writeln(sizeHashAndPath[i](2), " hashing ", sizeHashAndPath[i](1), " bytes");
      for j in i..next-1 {
        assert(sizeHashAndPath[i](2) == sizeHashAndPath[j](2));
        assert(sizeHashAndPath[i](1) == sizeHashAndPath[j](1));
        writeln("  ", sizeHashAndPath[j](3));
      }
    }
    i = next;
  }
}
