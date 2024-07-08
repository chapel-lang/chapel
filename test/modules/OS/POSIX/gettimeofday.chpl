use OS.POSIX;
use IO.FormattedIO;
var tv: struct_timeval;
writeln(gettimeofday(c_ptrTo(tv), nil));
writeln('%i.%06i'.format(tv.tv_sec:c_int, tv.tv_usec:c_int));
