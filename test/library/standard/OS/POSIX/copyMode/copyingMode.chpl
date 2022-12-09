use OS.POSIX;

proc myCopyMode(f1, f2) {
  var structStat: struct_stat;
  var err = stat(f1.c_str(), c_ptrTo(structStat));
  if err != 0 then halt("error in stat");
  err = chmod(f2.c_str(), structStat.st_mode);
  if err != 0 then halt("error in chmod");
}

var file1Name = "foo.txt";
var file2Name = "bar.txt";
// Preexec file causes these files to have different permissions beforehand.
myCopyMode(file1Name, file2Name);
// The files should match after this call.
