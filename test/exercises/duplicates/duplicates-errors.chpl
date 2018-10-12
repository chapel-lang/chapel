// This version is perhaps the most obvious / easiest to parallelize

use FileHashing;
use FileSystem;
use Sort;

config const filter = "";

// computeHashes is called by the below code to
// fill in the hash components of the array hashAndPath.
//
// hashAndPath is an array of tuples. Each array element
// has two tuple components:
//   component 1 is the hash
//   component 2 is the path
//
proc computeHashes(hashAndPath:[] (SHA256Hash, string)) {

  // The Tuples and Arrays primers might be useful
  // if you're not sure how to write this:

  // https://chapel-lang.org/docs/master/primers/tuples.html
  // https://chapel-lang.org/docs/master/primers/arrays.html

  // call computeFileHash to gather the hashes
  // computeFileHash is declared like this:
  //   computeFileHash(path: string): SHA256Hash throws
  // so it accepts a string argument and returns a SHA256Hash

  forall (hash, path) in hashAndPath {
    try {
      hash = computeFileHash(path);
    } catch e {
      writeln(e);
    }
  }
}

proc handleArguments(args: [] string, ref paths: domain(string)) {
  for arg in args[1..] {
    if isFile(arg) {
      if filter == "" || arg.endsWith(filter) {
        paths += relativeRealPath(arg);
      }
    } else if isDir(arg) {
      for path in findfiles(arg, recursive=true) {
        if filter == "" || path.endsWith(filter) {
          paths += relativeRealPath(path);
        }
      }
    } else {
      writeln("Error: argument not handled ", arg);
    }
  }
}

/*
 This routine looks for groups of adjacent entries in hashAndPath
 that have the same hash. When such a group is found with more
 than one element, it outputs the hash and then each filename
 with that hash.
 */
proc outputDuplicates(hashAndPath:[] (SHA256Hash, string)) {
  var i = 1;
  while i < hashAndPath.size {
    // Look for the group matching
    var next = i + 1;
    while next <= hashAndPath.size &&
          hashAndPath[i](1) == hashAndPath[next](1) {
      next += 1;
    }
    // Now i..next-1 is the group matching
    if i < next-1 {
      writeln(hashAndPath[i](1));
      for j in i..next-1 {
        assert(hashAndPath[i](1) == hashAndPath[j](1));
        writeln("  ", hashAndPath[j](2));
      }
    }
    i = next;
  }
}

/*
 The main routine is where the program starts running.
 */
proc main(args: [] string) {
  var paths:domain(string);

  // Find files based on arguments and store them in paths
  handleArguments(args, paths);

  // hashAndPath is an array of tuples. Each array element
  // has two tuple components:
  //   component 1 is the hash
  //   component 2 is the path
  var hashAndPath:[1..paths.size] (SHA256Hash, string);

  // This loop stores each of the values in `paths` into
  // the path component of an element in hashAndPath.
  //
  // While Chapel should eventually allow this pattern to
  // be expressed in parallel, it currently only works as a
  // `for` loop.
  //
  // See issues #11344 and #9437.
  for (tup, path) in zip(hashAndPath, paths) {
    tup(2) = path;
  }

  // Call the computeHashes function to fill in the SHA256Hash
  // component in each array element in hashAndPath.
  computeHashes(hashAndPath);

  // Use the sort library routine to sort these by hash by name.
  // This sort call relies on the default comparison function being
  // available for tuples.
  sort(hashAndPath);

  // Print out the duplicate files that we discovered by hashing
  // and sorting.
  outputDuplicates(hashAndPath);
}
