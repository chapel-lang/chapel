use CTypes;
var ret: int(32);

extern proc system(s: c_ptrConst(c_uchar)): int(32);

ret = system(c_ptrToConst_helper("/bin/echo hello"));
if ret != 0 then
  halt("echo failed");
