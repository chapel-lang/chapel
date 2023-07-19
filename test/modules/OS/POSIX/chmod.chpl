use OS.POSIX;
// The .preexec created these files.  Here, make their modes quite
// distinctive.  The .prediff will check the result.
writeln(chmod('chmod.file-1'.c_str(),
              S_IRUSR | S_IWGRP | S_IXOTH)); // these constants are single-bit
writeln(chmod('chmod.file-2'.c_str(),
              S_IRWXU | S_IRWXG | S_IRWXO)); // these constants are multi-bit
