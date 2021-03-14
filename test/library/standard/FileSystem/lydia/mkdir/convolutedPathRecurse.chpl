use FileSystem;
// This test attempts to make a directory using a path with ".." in it.
var dirname = "goDown2/../goUp2/stayPut";
mkdir(dirname, parents=true);
