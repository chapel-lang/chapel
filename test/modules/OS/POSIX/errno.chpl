use OS.POSIX;
var fildes = open(c_ptrToConst_helper('nonexistent-file'), O_RDONLY);
writeln(fildes);
if fildes < 0 {
  writeln(errno == ENOENT);
} else {
  writeln(close(fildes));
}
