// This is really a test of all five basic file I/O functions:
//   creat(), open(), read(), write(), close()
use OS.POSIX;

// Note that we can't check the access mode values we set here, because
// we don't know or do anything with the umask.  So really, we're just
// making sure the values we use don't cause an unexpected failure.

// creat(), file with mode -rwxrwxrwx, containing a line with "hello"
const fname_1 = 'open.file-1';
writeln(fname_1, ':');
var fildes = creat(fname_1.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
writeln(fildes >= 0);
writeln(POSIX.write(fildes, 'hello\n'.c_str():c_ptr(void), 6));
writeln(close(fildes));

param bufSize = 10;
var buf:c_array(c_char, bufSize);
fildes = open(fname_1.c_str(), O_RDONLY);
writeln(fildes >= 0);
writeln(POSIX.read(fildes, buf, bufSize));
writeln(string.createBorrowingBuffer(buf:c_ptrConst(c_char)));
writeln(close(fildes));

// open(), file with mode -rw-------, containing a line with "goodbye"
const fname_2 = 'open.file-2';
writeln(fname_2, ':');
fildes = open(fname_2.c_str(), O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
writeln(fildes >= 0);
writeln(POSIX.write(fildes, 'goodbye\n'.c_str():c_ptr(void), 8));
writeln(close(fildes));

fildes = open(fname_2.c_str(), O_RDONLY);
writeln(fildes >= 0);
writeln(POSIX.read(fildes, buf, bufSize));
writeln(string.createBorrowingBuffer(buf:c_ptrConst(c_char)));
writeln(close(fildes));

// re-open() the previous file, which should fail because O_EXCL
fildes = open(fname_2.c_str(), O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
writeln(fildes);
if fildes >= 0 {
  writeln(close(fildes));
}
