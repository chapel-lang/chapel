use FileSystem;
// This test looks into the creation of a directory using the mkdir command
// when the directory previously existed
var dirname = "already_present";
mkdir(dirname);
// Because there's a separate test for recursive directory creation.
