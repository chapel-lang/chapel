use FileSystem;

var filenames = ["blah.txt", "file", "dir", "file_link", "dir_link", "broken_link"];
for i in filenames {
  writeln(exists(i));
}
