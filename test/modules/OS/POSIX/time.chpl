use OS.POSIX;

writeln(time(nil):c_int);
var tValRefd:time_t;
var tVal = time(c_ptrTo(tValRefd));
writeln(tVal:c_int);
writeln(tVal:c_int == tValRefd:c_int);
