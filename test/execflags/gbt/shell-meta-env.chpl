use OS.POSIX;

const ev = getenv(c'SHELL_META_ENV');
if ev != c_nil then
  writeln(string.createWithNewBuffer(ev:c_string));
