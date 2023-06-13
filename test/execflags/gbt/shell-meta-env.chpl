use OS.POSIX;

const ev = getenv(c'SHELL_META_ENV');
if ev != nil then
  writeln(string.createCopyingBuffer(ev:c_string));
