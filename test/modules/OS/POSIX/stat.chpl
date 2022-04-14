use OS.POSIX;
use IO.FormattedIO;

var buf:struct_stat;
writeln(stat(c'stat.good', c_ptrTo(buf)) == 0);
//writeln('st_dev ', buf.st_dev:int);
//writeln('st_ino ', buf.st_ino:int);
writeln('st_mode ', buf.st_mode:int);
writeln('st_nlink ', buf.st_nlink:int);
writeln('st_uid ', buf.st_uid:int);
writeln('st_gid ', buf.st_gid:int);
writeln('st_rdev ', buf.st_rdev:int);
writeln('st_size ', buf.st_size:int);
writeln('st_atim %i.%09i'
        .format(buf.st_atim.tv_sec:int, buf.st_atim.tv_nsec:int));
writeln('st_mtim %i.%09i'
        .format(buf.st_mtim.tv_sec:int, buf.st_mtim.tv_nsec:int));
writeln('st_ctim %i.%09i'
        .format(buf.st_ctim.tv_sec:int, buf.st_ctim.tv_nsec:int));
writeln('st_blksize ', buf.st_blksize:int);
writeln('st_blocks ', buf.st_blocks:int);
