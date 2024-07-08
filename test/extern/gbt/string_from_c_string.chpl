use CTypes;
extern proc returns_c_string(): c_string;

writeln("Should be returned_c_string");
writeln(string.createCopyingBuffer(returns_c_string()));

extern proc returns_c_ptrConst(): c_ptrConst(c_char);

writeln("Should be returned_c_ptrConst");
writeln(string.createCopyingBuffer(returns_c_ptrConst()));
