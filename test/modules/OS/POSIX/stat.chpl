use OS.POSIX;
use IO.FormattedIO;

var buf:struct_stat;
writeln(stat('stat.good', c_ptrTo(buf)) == 0);
//writeln('st_dev ', buf.st_dev:c_int);
//writeln('st_ino ', buf.st_ino:c_int);
writeln('st_mode ', buf.st_mode:c_int);
writeln('st_nlink ', buf.st_nlink:c_int);
writeln('st_uid ', buf.st_uid:c_int);
writeln('st_gid ', buf.st_gid:c_int);
writeln('st_rdev ', buf.st_rdev:c_int);
writeln('st_size ', buf.st_size:c_int);
writeln('st_atim %i.%09i'
        .format(buf.st_atim.tv_sec:c_int, buf.st_atim.tv_nsec:c_int));
writeln('st_mtim %i.%09i'
        .format(buf.st_mtim.tv_sec:c_int, buf.st_mtim.tv_nsec:c_int));
writeln('st_ctim %i.%09i'
        .format(buf.st_ctim.tv_sec:c_int, buf.st_ctim.tv_nsec:c_int));
writeln('st_blksize ', buf.st_blksize:c_int);
writeln('st_blocks ', buf.st_blocks:c_int);

proc testconstant(arg) {
  // doesn't actually do anything -- just check that
  // we can compile uses of the constant
}

testconstant(S_IRWXU);
testconstant(S_IRUSR);
testconstant(S_IWUSR);
testconstant(S_IXUSR);

testconstant(S_IRWXG);
testconstant(S_IRGRP);
testconstant(S_IWGRP);
testconstant(S_IXGRP);

testconstant(S_IRWXO);
testconstant(S_IROTH);
testconstant(S_IWOTH);
testconstant(S_IXOTH);

testconstant(S_ISUID);
testconstant(S_ISGID);
testconstant(S_ISVTX);
