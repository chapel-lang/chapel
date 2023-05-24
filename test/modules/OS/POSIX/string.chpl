use OS.POSIX;
writeln(strlen(c''):int);
writeln(strlen(c'I am 18 bytes long'):int);

//writeln(strlen(c_ptrToConst_helper('')):int); // TODO: This should not segfault when using c_ptrConst
writeln(strlen(c_ptrToConst_helper('I am 18 bytes long')):int);
