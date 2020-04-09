use FileSystem;

// Tests moving a large and complicated tree structure
// I chose to implement the set up and some of the verification of this test
// outside of Chapel code due to this function's reliance on the other
// functions I would use to create this set up.
var src = "complexCopy";
var dest = "complexDest";

moveDir(src, dest);
writeln("original directory no longer exists: ", !exists(src));
