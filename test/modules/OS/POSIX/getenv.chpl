use OS.POSIX;
writeln(getenv(c'GETENV_TEST_ENV_VAR'):c_string:string);
const s = getenv(c'_GETENV_TEST_ENV_VAR'); // note underbar
writeln(if s == c_nil
        then 'this line is intentionally empty'
        else s:c_string:string);
