use OS.POSIX;

proc testconstant(arg) {
  // doesn't actually do anything
}

testconstant(O_ACCMODE);
testconstant(O_APPEND);
testconstant(O_CLOEXEC);
testconstant(O_CREAT);
testconstant(O_DIRECTORY);
testconstant(O_DSYNC);
testconstant(O_EXCL);
testconstant(O_NOCTTY);
testconstant(O_NOFOLLOW);
testconstant(O_NONBLOCK);
testconstant(O_RDONLY);
testconstant(O_RDWR);
testconstant(O_SYNC);
testconstant(O_TRUNC);
testconstant(O_WRONLY);

// Note: O_EXEC, O_SEARCH, O_TTY_INIT
// are documented in POSIX but don't seem to exist on linux
// Note: O_RSYNC
// is documented in POSIX but doesn't seem to exist on Mac OS
