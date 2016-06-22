use Path;

var invalidPath = "I/am/a/path"; // Verifies works on path that doesn't exist
writeln(basename(invalidPath));

var isDirname = "I/am/a/path/too/"; // Verifies that we get the empty string
writeln(basename(isDirname));

var empty = "";
// Verifies that we get the empty string back when we pass the empty string
writeln(basename(empty));

var onlyPath = pathSep;
// Verifies that we get the empty string back when we pass only our path
// separator
writeln(basename(onlyPath));

var withExtension = "blah/boop.txt";
// Verifies that we don't get confused by a .ext for a file
writeln(basename(withExtension));

var noDirname = "/boop.txt";
// Verifies we don't get confused when there is nothing to the left of the
// path separator
writeln(basename(noDirname));

var noDirDoubleSep = "//boop.txt";
// Verifies we don't get confused when there is only another path separator to
// the left of the last path separator
writeln(basename(noDirDoubleSep));
