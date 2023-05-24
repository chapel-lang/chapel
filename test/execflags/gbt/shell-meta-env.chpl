use OS.POSIX, CTypes;

const ev = getenv(c'SHELL_META_ENV');
if ev != nil then
  writeln(string.createCopyingBuffer(ev:c_ptrConst(c_uchar)));

const evv = getenv(c_ptrToConst_helper("SHELL_META_ENV"));
if evv != nil then
  writeln(string.createCopyingBuffer(evv));
