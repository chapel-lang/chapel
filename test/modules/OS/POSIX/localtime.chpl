use OS.POSIX;
use CTypes;

var t = time(nil);
var tLocal = localtime(c_ptrTo(t)).deref();
writeln(tLocal.tm_sec:int, ' ',
        tLocal.tm_min:int, ' ',
        tLocal.tm_hour:int, ' ',
        tLocal.tm_mday:int, ' ',
        tLocal.tm_mon:int, ' ',
        tLocal.tm_year:int, ' ',
        tLocal.tm_wday:int, ' ',
        tLocal.tm_yday:int, ' ',
        tLocal.tm_isdst:int);
writeln(asctime(c_ptrTo(tLocal)):c_string:string);

var tLocalRef = localtime_r(c_ptrTo(t), c_ptrTo(tLocal));
writeln(tLocalRef:c_intptr == c_ptrTo(tLocal):c_intptr);
writeln(tLocalRef.deref().tm_sec:int, ' ',
        tLocalRef.deref().tm_min:int, ' ',
        tLocalRef.deref().tm_hour:int, ' ',
        tLocalRef.deref().tm_mday:int, ' ',
        tLocalRef.deref().tm_mon:int, ' ',
        tLocalRef.deref().tm_year:int, ' ',
        tLocalRef.deref().tm_wday:int, ' ',
        tLocalRef.deref().tm_yday:int, ' ',
        tLocalRef.deref().tm_isdst:int);
param bufSize = 26; // can be found in man pages, but is not defined by POSIX!
var buf:c_array(c_char, bufSize);
var tAscRef = asctime_r(c_ptrTo(tLocal), buf);
writeln(tAscRef:c_intptr == buf:c_intptr);
writeln(tAscRef:c_string:string);
