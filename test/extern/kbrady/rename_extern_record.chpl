use SysCTypes, SysBasic;

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
