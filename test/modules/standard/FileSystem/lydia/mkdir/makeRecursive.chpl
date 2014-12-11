use FileSystem;
// This test looks into the creation of a directory using the mkdir command
// when we also wish to create parent directories
var dirname = "make/recursive/new/directory";
mkdir(dirname, parents=true);
