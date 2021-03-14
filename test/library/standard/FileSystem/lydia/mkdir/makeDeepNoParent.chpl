use FileSystem;
// This test looks into the creation of a directory using the mkdir command
// when one or more of the parent directories do not exist
var dirname = "make/deep/no/parent";
mkdir(dirname);
