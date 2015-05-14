use FileSystem;
use IO;

var original = "foo.txt";
var symlinkName = "foo2";
symlink(original, symlinkName); // Creates foo2 as a symlink of foo.txt
var f1 = open(original, iomode.r);
var f2 = open(symlinkName, iomode.r);
// Should match in both cases
writeln(sameFile(original, symlinkName));
writeln(sameFile(f1, f2));

f1.close();
f2.close();
remove(symlinkName);
