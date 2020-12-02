use CPtr;
use SysBasic, SysCTypes;

extern var y : c_ptr(c_int);

writeln(y[0]);
