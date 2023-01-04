use OS.POSIX;

var structStat: struct_stat;
var err = stat(c"blah", c_ptrTo(structStat));
if err != 0 then halt("error in stat");

writeln(structStat.st_mode:c_int & 0x1ff); // truncate to low 9 bits
// Blah should be all user permissions, or 448 in decimal
