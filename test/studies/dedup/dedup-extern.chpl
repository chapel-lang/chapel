use FileSystem;
use Subprocess;
use Sort;
use List;
use IO;
use CTypes;


// a SHA-1 hash is 160 bits, so it fits in 3 64-bit ints.
type Hash = (20*uint(8));

// This require statement indicates that the generated code
// should #include "openssl/sha.h" and be compiled with -lcrypto -lssl
require "openssl/sha.h", "-lcrypto", "-lssl";
// This 'extern proc' declaration tells the Chapel compiler that a C
// function SHA1 is available and describes the arguments in the
// Chapel type system.
extern proc SHA1(d:c_ptr(uint(8)), n:c_size_t, md:c_ptr(uint(8)));

proc main(args:[] string)
{
  var paths: list(string);

  for arg in args[1..] {
    if isFile(arg) then
      paths.pushBack(arg);
    else if isDir(arg) then
      for path in findFiles(arg, recursive=true) do
        paths.pushBack(path);
  }

  // Create an array of hashes and file ids
  // a file id is just the index into the paths array.
  var hashAndFileId:[0..#paths.size] (Hash, int);

  // Compute the SHA1 sums using the extern calls
  var pathsArray = paths.toArray();
  forall (id,path) in zip(pathsArray.domain, pathsArray) {
    var mdArray:[0..19] uint(8);
    var data:string;
    var f = open(path, ioMode.r);
    f.reader(kind=iokind.native).readAll(data);
    SHA1(data.c_str():c_ptr(uint(8)), data.numBytes:uint, c_ptrTo(mdArray));
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
