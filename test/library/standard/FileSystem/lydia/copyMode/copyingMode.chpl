use FileSystem;

var file1Name = "foo.txt";
var file2Name = "bar.txt";
// Preexec file causes these files to have different permissions beforehand.
copyMode(file1Name, file2Name);
// The files should match after this call.
