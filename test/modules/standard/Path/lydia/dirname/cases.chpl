use Path;

var invalidPath = "I/am/a/path"; // Verifies works on path that doesn't exist
writeln(dirname(invalidPath));

var isDirname = "I/am/a/path/too/"; // Verifies that we get the empty string
writeln(dirname(isDirname));

var empty = "";
// Verifies that we get the empty string back when we pass the empty string
writeln(dirname(empty));

var onlyPath = pathSep;
// Verifies that we get the empty string back when we pass only our path
// separator
writeln(dirname(onlyPath));

var withExtension = "blah/boop.txt";
// Verifies that we don't get confused by a .ext for a file
writeln(dirname(withExtension));

var noDirname = "/boop.txt";
// Verifies we don't get confused when there is nothing to the left of the
// path separator
writeln(dirname(noDirname));

var noDirDoubleSep = "//boop.txt";
// Verifies we don't get confused when there is only another path separator to
// the left of the last path separator
writeln(dirname(noDirDoubleSep));

var dirDoubleSep = "bar//boop.txt";
// Verifies we don't get confused when there two path separators between the
// directory name and the file name.
writeln(dirname(dirDoubleSep));
