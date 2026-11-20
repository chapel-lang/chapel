use CTypes;

extern proc MACRO_STRINGIFY(a: c_int): c_ptrConst(c_char);
writeln(string.createCopyingBuffer(MACRO_STRINGIFY(1:c_int)).find("1") != -1);
