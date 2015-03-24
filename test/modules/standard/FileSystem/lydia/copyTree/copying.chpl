use FileSystem;

copyTree("complexSrc", "complexDest"); // copies a large tree
copyTree("complexSrc/emptyDir", "emptyDest");
// copies an empty directory
copyTree("complexSrc/hasDirWithFileOnly", "sparseDest");
// copies a directory whose only contents is another directory with
// a file in it.
copyTree("complexSrc/hasEmptyDir", "verySparseDest");
// copies a directory whose only contents is another directory that
// is empty.
copyTree("complexSrc/hasFile", "shortDest");
// copies a directory whose only contents is a file
copyTree("complexSrc/hasFileAndDirWithFile", "denseDest");
// copies a directory which contains a file and another directory,
// which also has a file in it.
copyTree("complexSrc/hasFileAndEmptyDir", "cutoffDest");
// copies a directory which contains a file and another directory,
// that is empty.
