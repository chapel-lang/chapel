use FileSystem;
use Spawn;
use Sort;
use BlockDist;
use VisualDebug;
use List;

config const verbose = false;
config const vis = "vis_ints";
config const visualize = false;

// a SHA-1 hash is 160 bits, so it fits in 3 64-bit ints.
type Hash = (int,int,int);

proc main(args:[] string)
{
  var paths: list(string);

  for arg in args[1..] {
    if isFile(arg) then
      paths.append(arg);
    else if isDir(arg) then
      for path in findfiles(arg, recursive=true) do
        paths.append(path);
  }

  var n:int = paths.size;
  var BlockN = {0..#n} dmapped Block({0..#n});
  var distributedPaths:[BlockN] string;
  distributedPaths = paths.toArray();
 
  // Create an array of hashes and file ids
  // a file id is just the index into the paths array.
  var hashAndFileId:[0..#paths.size] (Hash, int);
 
  if visualize then
    startVdebug(vis);

  // Compute the SHA1 sums using the external program
  forall (id,path) in zip(distributedPaths.domain, distributedPaths) {
    if verbose then
      writeln("Running sha1sum ", path);
    var sub = spawn(["sha1sum", path], stdout=PIPE);
    var hashString:string;
    sub.stdout.read(hashString);
    if verbose then
      writeln(hashString, " ", path);
    var hash = stringToHash(hashString);
    // This version is just communicating 4 integer values
    // back to Locale 0.
    hashAndFileId[id] = (hash, id);
    sub.wait();
  }

  if visualize then
    stopVdebug();

  if verbose then
    writeln("Sorting by hash to find duplicates");

  // Sort the hashAndFileNum array
  sort(hashAndFileId);

  if verbose then
    writeln("Outputting Duplicates");

  var lasthash, hash : Hash;
  var id, lastid : int;
  var ingroup = false; 
  for (hash, id) in hashAndFileId {
    if hash == lasthash {
      writeln(paths[lastid]);
      ingroup = true;
    } else if ingroup {
      writeln(paths[lastid]);
      writeln();
      ingroup = false;
    }
    lastid = id;
    lasthash = hash;
  }
  if ingroup {
    writeln(paths[lastid]);
    writeln();
  }
}


proc stringToHash(s:string): Hash {
  // The below is a workaround since Chapel doesn't yet have
  // an equivalent of sscanf in C and readf for integers
  // can't take in a maximum field width

  // Open up an in-memory "file"
  var f = openmem();
  var w = f.writer();
  // Write int-sized substrings separated by spaces
  w.write(s[1..16], " ");
  w.write(s[17..32], " ");
  w.write(s[17..32]);
  w.close();
  var r = f.reader();
  var hash:Hash;
  // Use Formatted I/O to read hex values into integers
  r.readf("%xu%xu%xu", hash(0), hash(1), hash(2));
  r.close();
  return hash;
}



