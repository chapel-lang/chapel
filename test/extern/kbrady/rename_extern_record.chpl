use CTypes, OS.POSIX;

// Note that OS.POSIX itself supplies a `struct stat` record and
// `stat()` call, so this is to some extent superfluous.  But the
// purpose of that is an OS interface, while the purpose of this
// is a check of our renamed external struct support, and reusing
// the existing `struct stat` is by far the easiest thing to do
// here.  And the two uses don't interfere, so all is well.
extern "struct stat" record chpl_stat {
  var st_size: off_t;
}

proc getFileSize(path:c_string) : int {
  extern proc stat(x: c_string, ref chpl_stat): c_int;
  var buf: chpl_stat;

  if (stat(path, buf) == 0) {
    return buf.st_size;
  }
  return -1;
}

writeln(getFileSize("./rename_extern_record.good"));
