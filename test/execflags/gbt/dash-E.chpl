use CTypes;
extern proc getenv(const name: c_ptrConst(c_char)): c_ptrConst(c_char);
writeln(string.createCopyingBuffer(getenv('DASH_E_ENV_VAR')));
