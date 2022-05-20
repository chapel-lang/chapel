use OS.POSIX;
// The .preexec created these files.  Here, make their modes quite
// distinctive.  The .prediff will check the result.
writeln(chmod(c'chmod.file-1',
              S_IRUSR | S_IWGRP | S_IXOTH)); // these constants are single-bit
writeln(chmod(c'chmod.file-2',
              S_IRWXU | S_IRWXG | S_IRWXO)); // these constants are multi-bit
