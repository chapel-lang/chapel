use FileSystem;
use BlockDist;
use Spawn;

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

  // Now create a distributed-memory version of paths.

  var n:int = paths.size;
  var BlockN = {1..n} dmapped Block({1..n});
  var distributedPaths:[BlockN] string;
  distributedPaths = paths;
  var BlockNumLocales = {0..#numLocales} dmapped Block({0..#numLocales});
  var distributedBuffers: [BlockNumLocales] file;
  var distributedWriters: [BlockNumLocales]
    channel(writing=true, kind=iokind.native, locking=true);
  
  // Open up buffers to store the hashes 
  forall (f,w) in zip(distributedBuffers, distributedWriters) {
    f = openmem();
    w = f.writer();
  }
  // Compute the SHA1 sums using the external program
  // Do so in parallel across all locales
  forall path in distributedPaths {
    var sub = spawn(["sha1sum", path], stdout=PIPE);
    var hash:string;
    sub.stdout.read(hash);
    distributedWriters[here.id].writeln(hash, " ", path);
    sub.wait();
  }
  // Close the channels now that we have finished writing
  for w in distributedWriters do
    w.close();
 
  var sorter = spawn(["sort"], stdin=PIPE, stdout=PIPE);
  cobegin {
    {
      // Output all of the to the sort process
      for f in distributedBuffers {
        on f do {
          var data:string;
          var reader = f.reader();
          reader.readstring(data);
          sorter.stdin.writeln(data);
        }
      }
      sorter.stdin.close();
    }
    {
      // Read data from the sort process
      var lasthash, hash, path, lastpath : string;
      var ingroup = false; 
      while sorter.stdout.read(hash, path) {
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
  }

  sorter.wait();
}



