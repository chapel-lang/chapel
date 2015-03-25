use FileSystem;
// This test looks into the creation of a directory using the mkdir command
// when we also wish to create parent directories
var dirname = "make/recursive/new/directory";
mkdir(dirname, parents=true);
// This also guarantees that the recursive call will not fail if any or all
// of the directories listed have previously been created.
mkdir(dirname, parents=true); // This call should not fail.
