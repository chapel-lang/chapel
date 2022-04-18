use OS.POSIX;

const ev = getenv(c'SHELL_META_ENV');
if ev != c_nil then
  writeln(createStringWithNewBuffer(ev:c_string));
