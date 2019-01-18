use FileSystem;

copyTree("blah.txt", "thatWasntADirectory");
// As probably gathered from the name, blah.txt is in fact not a directory.
// copyTree would not work on it.
