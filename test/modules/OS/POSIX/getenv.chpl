use OS.POSIX;
writeln(string.createBorrowingBuffer(getenv('GETENV_TEST_ENV_VAR')));
const sPtr = getenv('_GETENV_TEST_ENV_VAR'); // note underbar
writeln(if sPtr == nil
        then 'this line is intentionally empty'
        else string.createBorrowingBuffer(sPtr));
