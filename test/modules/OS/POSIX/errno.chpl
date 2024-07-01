module testErrno {
  use OS.POSIX;
  var fildes = open('nonexistent-file'.c_str(), O_RDONLY);
  writeln(fildes);
  if fildes < 0 {
    writeln(errno == ENOENT);
  } else {
    writeln(close(fildes));
  }
}
