use SysCTypes;

// This used to result in an internal error during compilation,
// due to a bug in function resolution.

enum myEnum { a=1:c_int, b=2:c_int, c=3:c_int};
