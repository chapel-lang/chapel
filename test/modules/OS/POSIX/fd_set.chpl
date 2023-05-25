use OS.POSIX;

param fdMax = 2;
writeln(FD_SETSIZE > fdMax);

var fdset:fd_set;

FD_ZERO(c_ptrTo(fdset));
writeln(FD_ISSET(0, c_ptrTo(fdset)) != 0);

FD_SET(fdMax, c_ptrTo(fdset));
writeln(FD_ISSET(0, c_ptrTo(fdset)) != 0);
writeln(FD_ISSET(fdMax, c_ptrTo(fdset)) != 0);

FD_SET(0, c_ptrTo(fdset));
FD_CLR(fdMax, c_ptrTo(fdset));
writeln(FD_ISSET(0, c_ptrTo(fdset)) != 0);
writeln(FD_ISSET(fdMax, c_ptrTo(fdset)) != 0);
