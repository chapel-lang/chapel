use FileSystem;
use Spawn;
use Sort;
use List;

config const verbose = false;

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

  // Create an array of hashes and file ids
  // a file id is just the index into the paths array.
  var hashAndFileId:[1..paths.size] (Hash, int);
 
  // Compute the SHA1 sums using the external program
  var pathsArray = paths.toArray();
  forall (id,path) in zip(pathsArray.domain, pathsArray) {
    if verbose then
      writeln("Running sha1sum ", path);
    var sub = spawn(["sha1sum", path], stdout=PIPE);
    var hashString:string;
    sub.stdout.read(hashString);
    if verbose then
      writeln(hashString, " ", path);
    var hash = stringToHash(hashString);
    hashAndFileId[id] = (hash, id);
    sub.wait();
  }

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
  // TODO: could be improved by:
  //  * loop over characters in a string
  //  * equivalent of sscanf
  //  * readf for integers with a maximum field width
  var f = openmem();
  var w = f.writer();
  w.write(s[1..16], " ");
  w.write(s[17..32], " ");
  w.write(s[17..32]);
  w.close();
  var r = f.reader();
  var hash:Hash;
  r.readf("%xu%xu%xu", hash(0), hash(1), hash(2));
  r.close();
  return hash;
}



