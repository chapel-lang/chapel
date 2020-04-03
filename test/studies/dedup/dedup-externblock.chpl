use FileSystem;
use Spawn;
use Sort;
use List;
use IO;

// a SHA-1 hash is 160 bits, so it fits in 3 64-bit ints.
type Hash = (20*uint(8));

// This require statement allows this module to add
// some required libraries to the link line
require "-lcrypto", "-lssl";

// The extern block allows Chapel source code to include C declarations.
// The declarations are automatically added to the enclosing Chapel
// scope. Functions, variables, and types are supported - including
// inline functions. Macros have limited support.
// See
//   https://chapel-lang.org/docs/technotes/extern.html
extern {
  #include <openssl/sha.h>
}


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

  // Compute the SHA1 sums using the extern calls
  var pathsArray = paths.toArray();
  forall (id,path) in zip(pathsArray.domain, pathsArray) {
    var mdArray:[0..19] uint(8);
    var data:string;
    var f = open(path, iomode.r);
    f.reader(kind=iokind.native).readstring(data);
    // The extern block above included everything in openssl/sha.h,
    // including the SHA1 function. But, in order to call it, we
    // need to create C types from some Chapel data.
    //   string.c_str() returns a C string referring to the string's data
    //   c_ptrTo(something) returns a C pointer referring to something
    SHA1(data.c_str(), data.numBytes:uint, c_ptrTo(mdArray));
    var hash:Hash;
    for i in 0..19 do
      hash(i) = mdArray(i);
    hashAndFileId[id] = (hash, id);
  }

  // Sort the hashAndFileNum array
  sort(hashAndFileId);

  // Read data from the sort process
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
