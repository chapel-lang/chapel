use FileSystem;

var filename = "foo.txt";
var newName = "bar.txt";
symlink(filename, newName);
writeln(isLink(newName));
