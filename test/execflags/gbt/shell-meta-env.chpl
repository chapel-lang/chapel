use OS.POSIX, CTypes;

const evv = getenv("SHELL_META_ENV");
if evv != nil then
  writeln(string.createCopyingBuffer(evv));
