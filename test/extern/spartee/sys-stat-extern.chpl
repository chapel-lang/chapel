
extern "struct sys_stat_s" record chpl_stat {
  var st_mtimespec: off_t;
}

proc getLastModified(filename: string) : int {
  extern proc sys_stat(filename: c_string, ref chpl_stat): c_int;

  var file_buf: chpl_stat;
  var file_path = filename.c_str();

  if (sys_stat(file_path, file_buf) == 0) {
    return file_buf.st_mtimespec;
    }
  return -1;
}

writeln(getLastModified("sys-stat-extern.chpl"));