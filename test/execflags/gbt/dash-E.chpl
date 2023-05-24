use CTypes;
extern proc getenv(const name: c_ptrConst(c_uchar)): c_ptrConst(c_uchar);
writeln(string.createCopyingBuffer(getenv(c_ptrToConst_helper('DASH_E_ENV_VAR'))));
