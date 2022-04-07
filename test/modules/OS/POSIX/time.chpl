use OS.POSIX;
use CTypes;

writeln(time(nil):int);
var tValRefd:time_t;
var tVal = time(c_ptrTo(tValRefd));
writeln(tVal:int);
writeln(tVal:int == tValRefd:int);
