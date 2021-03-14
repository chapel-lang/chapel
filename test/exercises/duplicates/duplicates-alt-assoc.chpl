// This version uses an associative array instead of sorting

use FileHashing;
use FileSystem;
use Sort;
use Time;

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

  // Let's make hashes map from SHA256Hash to an array of paths
  // stumbling block: skyline arrays
  // stumbling block: declaring associative arrays
  var hashes:domain(SHA256Hash);
  var hashesToPaths:[hashes] domain(string);

  if timing {
    writeln("Reading and hashing files");
  }
  clock.clear();
  clock.start();

  for path in paths {
    // stumbling block: how to add to an associative array?
    // stumbling block: making this parallel probably causes failures
    // stumbling block: associative domains parSafe
    var hash = computeFileHash(path);
    hashes += hash;
    hashesToPaths[hash] += path;
  }

  clock.stop();
  if timing {
    writeln("Reading and hashing files took ", clock.elapsed(), " seconds");
  }

  // stumbling block: associative array key+value iteration
  // stumbling block: associative array key+value iteration in sorted order
  writeln("Duplicate files found:");
  for hash in hashes.sorted() {
    if hashesToPaths[hash].size > 1 {
      writeln(hash);
      for path in hashesToPaths[hash].sorted() {
        writeln("  ", path);
      }
    }
  }
}
