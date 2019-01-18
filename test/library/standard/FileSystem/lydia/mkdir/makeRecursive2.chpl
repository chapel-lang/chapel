use FileSystem;
// This test looks into the creation of a directory using the mkdir command
// when we also wish to create parent directories and the name ends with a 
// slash.
var dirname = "make2/recursive/new/directory/";
mkdir(dirname, parents=true);
