use SysCTypes;

extern "struct sys_stat_s" record chpl_stat {
  var st_mtim: chpl_timespec;
}

extern "struct timespec" record chpl_timespec {
  var tv_sec: int;
  var tv_nsec: int;
}

proc getLastModified(filename: string) : int {
  extern proc sys_stat(filename: c_string, ref chpl_stat): c_int;

  var file_buf: chpl_stat;
  var file_path = filename.c_str();

  if (sys_stat(file_path, file_buf) == 0) {
    return file_buf.st_mtim.tv_sec;
    }
  return -1;
}

getLastModified("sys-stat-extern.chpl");
writeln();

