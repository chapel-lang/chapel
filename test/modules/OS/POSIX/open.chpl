// This is really a test of all five basic file I/O functions:
//   creat(), open(), read(), write(), close()
use OS.POSIX;
use CTypes;

// Note that we can't check the access mode values we set here, because
// we don't know or do anything with the umask.  So really, we're just
// making sure the values we use don't cause an unexpected failure.

// creat(), file with mode -rwxrwxrwx, containing a line with "hello"
const fname_1 = c'open.file-1';
writeln(fname_1:string, ':');
var fildes = creat(fname_1, S_IRWXU | S_IRWXG | S_IRWXO);
writeln(fildes >= 0);
writeln(POSIX.write(fildes, c'hello\n':c_void_ptr, 6));
writeln(close(fildes));

param bufSize = 10;
var buf:c_array(c_char, bufSize);
fildes = open(fname_1, O_RDONLY);
writeln(fildes >= 0);
writeln(POSIX.read(fildes, buf, bufSize));
writeln((buf:c_string):string);
writeln(close(fildes));

// open(), file with mode -rw-------, containing a line with "goodbye"
const fname_2 = c'open.file-2';
writeln(fname_2:string, ':');
fildes = open(fname_2, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
writeln(fildes >= 0);
writeln(POSIX.write(fildes, c'goodbye\n':c_void_ptr, 8));
writeln(close(fildes));

fildes = open(fname_2, O_RDONLY);
writeln(fildes >= 0);
writeln(POSIX.read(fildes, buf, bufSize));
writeln((buf:c_string):string);
writeln(close(fildes));

// re-open() the previous file, which should fail because O_EXCL
fildes = open(fname_2, O_CREAT | O_EXCL | O_WRONLY, S_IRUSR | S_IWUSR);
writeln(fildes);
if fildes >= 0 {
  writeln(close(fildes));
}
