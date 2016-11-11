use FileSystem;
use Spawn;
use Sort;

// a SHA-1 hash is 160 bits, so it fits in 3 64-bit ints.
type Hash = (20*uint(8));

require "openssl/sha.h", "-lcrypto", "-lssl";
extern proc SHA1(d:c_string, n:size_t, md:c_ptr(uint(8)));

proc main(args:[] string)
{
  var paths:[1..0] string;

  for arg in args[1..] {
    if isFile(arg) then
      paths.push_back(arg);
    else if isDir(arg) then
      for path in findfiles(arg, recursive=true) do
        paths.push_back(path);
  }

  // Create an array of hashes and file ids
  // a file id is just the index into the paths array.
  var hashAndFileId:[1..paths.size] (Hash, int);

  // Compute the SHA1 sums using the extern calls
  forall (id,path) in zip(paths.domain, paths) {
    var mdArray:[1..20] uint(8);
    var data:string;
    var f = open(path, iomode.r);
    f.reader(kind=iokind.native).readstring(data);
    SHA1(data.c_str(), data.length:uint, c_ptrTo(mdArray));
    var hash:Hash;
    for i in 1..20 do
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
