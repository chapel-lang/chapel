/* Note: the .good output was generated with the following bash script:

for i in {2..201}
do
 echo $((i-1))
 seq -sa $i | tr -d '[:digit:]' | sha256sum;
done

*/

use FileHashing;
use FileSystem;
use IO;

config const path = "many-a.tmp";

// Print hash of n "a" characters
proc printHashOfN(n:int) {
  // Fill out the "a"s file
  {
    var f = open(path, iomode.cwr);
    var w = f.writer();
    for i in 1..n {
      w.write("a");
    }
    w.writeln();
    // closes w, f
  }

  var hash = computeFileHash(path);
  writeln(n);
  writeln(hash, "  -");

  remove(path);
}

for i in 1..200 {
  printHashOfN(i);
}
