use OS.POSIX, CTypes;

const ev = getenv(c'SHELL_META_ENV');
if ev != nil then
  writeln(string.createCopyingBuffer(ev));

const evv = getenv("SHELL_META_ENV");
if evv != nil then
  writeln(string.createCopyingBuffer(evv));
