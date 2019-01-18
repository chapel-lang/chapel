use Path;

var invalidPath = "I/am/a/path"; // Verifies works on path that doesn't exist
writeln(splitPath(invalidPath));

var isDirName = "I/am/a/path/too/"; // Verifies that we get the empty string
writeln(splitPath(isDirName));

var empty = "";
// Verifies that we get the empty string back when we pass the empty string
writeln(splitPath(empty));

var onlyPath = pathSep;
// Verifies that we get the empty string back when we pass only our path
// separator
writeln(splitPath(onlyPath));

var withExtension = "blah/boop.txt";
// Verifies that we don't get confused by a .ext for a file
writeln(splitPath(withExtension));

var noDirName = "/boop.txt";
// Verifies we don't get confused when there is nothing to the left of the
// path separator
writeln(splitPath(noDirName));

var noDirDoubleSep = "//boop.txt";
// Verifies we don't get confused when there is only another path separator to
// the left of the last path separator
writeln(splitPath(noDirDoubleSep));

var dirDoubleSep = "bar//boop.txt";
// Verifies we don't get confused when there two path separators between the
// directory name and the file name.
writeln(splitPath(dirDoubleSep));

var smallDiff = "/a/foo.txt";
// Verifies I didn't mess up detection when trying to handle the noDirDoubleSep
// case
writeln(splitPath(smallDiff));

var onlyFile = "foo.txt";
// Verifies we aren't confused by the lack of the path separator symbol.
writeln(splitPath(onlyFile));
