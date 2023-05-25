use FileSystem;
use Subprocess;
use Sort;
use BlockDist;
use VisualDebug;
use List;

config const verbose = false;
config const vis = "vis_strings";
config const visualize = false;

proc main(args:[] string)
{
  // This program looks for duplicate files.
  // Arguments are files or directories to include in search.

  // Gather the paths we want to hash to find duplicates.
  // Start out with a 0-length array
  // We'll append to it with pushBack
  // This is only possible for arrays that do not share a domain.
  var paths: list(string);

  for arg in args[1..] {
    if isFile(arg) then
      paths.pushBack(arg);
    else if isDir(arg) then
      // use FileSystem.findFiles to easily enumerate files.
      // A parallel version is available.
      for path in findFiles(arg, recursive=true) do
        paths.pushBack(path);
  }

  // Create a distributed array of paths so that we can distribute the
  // work of hashing files to different Locales
  var n:int = paths.size;
  var BlockN = {1..n} dmapped Block({1..n});
  var distributedPaths:[BlockN] string;
  distributedPaths = paths.toArray();
 
  // Create an array of hashes paths.
  // This array is not distributed in this version.
  // The array will store (hash, path).
  // After computing this array, we'll sort it in order to
  // find duplicates.
  var hashAndFile:[1..paths.size] (string, string);
 
  if visualize then
    startVdebug(vis);

  // Using the Spawn module, compute the SHA1 sums using an external program
  forall (id,path) in zip(distributedPaths.domain, distributedPaths) {
    if verbose then
      writeln("Running sha1sum ", path);
    // The spawn call creates a subprocess. By specifying
    // stdout=pipeStyle.pipe, we are requesting that the output of the
    // subprocess be sent to a pipe that we can read from.
    var sub = spawn(["sha1sum", path], stdout=pipeStyle.pipe);
    // Read the hash value from the output of sha1sum.
    // Note that sha1sum output looks like this:
    // d556d22d3e7b3ae55108442b36b5833523c923b7  dedup-distributed-strings.chpl
    var hashString:string;
    sub.stdout.read(hashString);
    if verbose then
      writeln(hashString, " ", path);
    // Store the hash and the path into the array.
    // Since the array is not distributed, this sends data to Locale 0.
    hashAndFile[id] = (hashString, path);
    sub.wait();
  }

  if visualize then
    stopVdebug();

  if verbose then
    writeln("Sorting by hash to find duplicates");

  // Sort the hashAndFile array on Locale 0
  // Since we stored the hash value first in the tuple elements,
  // this call groups values with the same hash.
  sort(hashAndFile);

  if verbose then
    writeln("Outputting Duplicates");

  // Print out any duplicate files in groups by finding values
  // with the same hash and printing out the paths.
  // Separate each group with a blank line
  var lasthash, hash : string;
  var path, lastpath : string;
  var ingroup = false; 
  for (hash, path) in hashAndFile {
    if hash == lasthash {
      writeln(lastpath);
      ingroup = true;
    } else if ingroup {
      writeln(lastpath);
      writeln();
      ingroup = false;
    }
    lastpath = path;
    lasthash = hash;
  }
  if ingroup {
    writeln(lastpath);
    writeln();
  }
}
