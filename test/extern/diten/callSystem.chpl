var ret: int(32);

extern proc system(s: c_string): int(32);

ret = system(c"/bin/echo hello");
if ret != 0 then
  halt("echo failed");
