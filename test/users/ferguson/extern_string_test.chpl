use CTypes;
extern proc return_string_test():c_ptrConst(c_char);
extern proc return_string_arg_test(ref str: c_ptrConst(c_char));

writeln("returned string ", string.createCopyingBuffer(return_string_test()));
var cs: c_ptrConst(c_char);
return_string_arg_test(cs);
var s = string.createCopyingBuffer(cs);
writeln("returned string arg ",s);

