// This version uses an associative array instead of sorting
// and works in parallel by + reducing the associative array.

use FileHashing;
use FileSystem;
use Sort;
use Time;

config const timing = false;

record Set {
  type keyType;
  var keys: domain(keyType);
}

proc +(a:Set(?t), b:Set(t)) {
  var sum = new Set(t);
  sum.keys += a.keys;
  sum.keys += b.keys;
  return sum;
}

record Map {
  type keyType;
  type valueType;
  var keys: domain(keyType);
  var values: [keys] valueType;
  proc this(key:keyType) ref : valueType {
    return values[key];
  }
}

proc +(a:Map(?t,?u), b:Map(t,u)) {
  var sum = new Map(t,u);
  sum.keys += a.keys;
  sum.keys += b.keys;
  for key in a.keys {
    ref sumElt = sum.values[key];
    sumElt = sumElt + a.values[key];
  }
  for key in b.keys {
    ref sumElt = sum.values[key];
    sumElt = sumElt + b.values[key];
  }
  return sum;
}


class CombiningReduceOp: ReduceScanOp {
  type eltType;

  var state: Map(SHA256Hash, Set(string));

  proc identity {
    var ret:eltType;
    return ret;
  }
  proc accumulate(elm:Map) {
    state.keys += elm.keys;
    for hash in elm.keys {
      state.values[hash].keys += elm.values[hash].keys;
    }
  }
  proc accumulateOntoState(ref state, elm:_tuple) {
    var hash = elm(0);
    var path = elm(1);
    state.keys += hash;
    state.values[hash].keys += path;
  }
  proc combine(other:CombiningReduceOp) {
    state.keys += other.state.keys;
    for hash in other.state.keys {
      state.values[hash] += other.state.values[hash];
    }
  }
  proc generate() {
    return state;
  }
  proc clone() {
    return new unmanaged CombiningReduceOp(eltType=eltType);
  }
}

proc main(args: [] string) {
  var clock:Timer;
  var paths:Set(string);

  if timing {
    writeln("Gathering files");
  }
  clock.start();
  for arg in args[1..] {
    if isFile(arg) {
      paths.keys += relativeRealPath(arg);
    } else if isDir(arg) {
      for path in findfiles(arg, recursive=true) {
        paths.keys += relativeRealPath(path);
      }
    } else {
      writeln("Error: argument not handled ", arg);
    }
  }
  clock.stop();
  if timing {
    writeln("Gathering ", paths.keys.size, " files took ", clock.elapsed(), " seconds");
  }

  // Let's make hashes map from SHA256Hash to an array of paths
  // stumbling block: skyline arrays
  // stumbling block: declaring associative arrays
  var hashesToPaths: Map(SHA256Hash, Set(string));

  if timing {
    writeln("Reading and hashing files");
  }
  clock.clear();
  clock.start();

  forall path in paths.keys with (CombiningReduceOp reduce hashesToPaths) {
    // stumbling block: how to add to an associative array?
    // stumbling block: making this parallel probably causes failures
    // stumbling block: associative domains parSafe
    var hash = computeFileHash(path);
    hashesToPaths reduce= (hash, path);
  }

  clock.stop();
  if timing {
    writeln("Reading and hashing files took ", clock.elapsed(), " seconds");
  }

  // stumbling block: associative array key+value iteration
  // stumbling block: associative array key+value iteration in sorted order
  writeln("Duplicate files found:");
  for hash in hashesToPaths.keys.sorted() {
    if hashesToPaths.values[hash].keys.size > 1 {
      writeln(hash);
      for path in hashesToPaths.values[hash].keys.sorted() {
        writeln("  ", path);
      }
    }
  }
}
