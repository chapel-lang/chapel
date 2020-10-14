use Sys;

var ev: c_string;
if sys_getenv('SHELL_META_ENV', ev) != 0 then
  writeln(createStringWithNewBuffer(ev));
