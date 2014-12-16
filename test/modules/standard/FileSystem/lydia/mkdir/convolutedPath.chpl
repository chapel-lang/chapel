use FileSystem;
// This test attempts to make a directory using a path with ".." in it.
var dirname = "goDown/../goUp/stayPut";
mkdir(dirname);
