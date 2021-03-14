use FileSystem;
// Fails to move the file no_such_fileOld.txt to no_such_fileNew.txt
// because no_such_fileOld.txt does not exist.
var oldname = "no_such_fileOld.txt";
var newname = "no_such_fileNew.txt";
rename(oldname, newname);
