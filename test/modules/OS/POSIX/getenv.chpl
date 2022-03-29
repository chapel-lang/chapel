use OS.POSIX;
writeln(getenv(c'GETENV_TEST_ENV_VAR'):string);
const s = getenv(c'_GETENV_TEST_ENV_VAR'):string;
writeln(if s.isEmpty()
        then 'this line is intentionally empty'
        else s);
